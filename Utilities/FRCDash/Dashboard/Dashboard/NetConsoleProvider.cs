﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

using TomShane.Neoforce.Controls;
using nfConsole = TomShane.Neoforce.Controls.Console;

namespace Dashboard
{
    public class NetConsoleProvider
    {
        private nfConsole _console;

        private object _blockMutex;
        private bool _block;

        private Queue<string> _rcvQ;
        private object _rcvQMutex;

        private Queue<string> _errorQ;
        private object _errorQMutex;

        private Thread _rcvThread;

        Socket _socket;
        EndPoint _robotEP;

        public NetConsoleProvider(nfConsole console, string remoteIP)
        {
            _block = false;
            _blockMutex = new Object();

            _console = console;

            _console.MessageSent += new ConsoleMessageEventHandler(ConsoleCommandSent);

            _rcvQ = new Queue<string>();
            _rcvQMutex = new object();

            _errorQ = new Queue<string>();
            _errorQMutex = new object();

            _rcvThread = new Thread(ReceiveTask);
            _rcvThread.IsBackground = true;

            try
            {
                IPEndPoint remote = new IPEndPoint(IPAddress.Any, 6666);
                _robotEP = new IPEndPoint(IPAddress.Parse(remoteIP), 6668);

                _socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

                _socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                _socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, true);

                _socket.Bind(remote);
            }
            catch (SocketException se)
            {
                lock (_errorQMutex)
                {
                    _errorQ.Enqueue(se.ErrorCode + ": " + se.Message + (se.ErrorCode == 10013 ? " - is another Net Console running?" : ""));
                }
            }
            catch (Exception e)
            {
                lock (_errorQMutex)
                {
                    _errorQ.Enqueue(e.Message);
                }
            }
        }

        public void Start()
        {
            _rcvThread.Start();
        }

        public void Stop()
        {
            _rcvThread.Abort();
        }

        private string parseTabs(string s)
        {
            string ret = "";
            string[] p = s.Split('\t');

            foreach (string t in p)
            {
                ret += t;
                for (int i = 0; i < 4 - (t.Length % 4); i++)
                {
                    ret += ' ';
                }
            }

            return ret;
        }

        private void ReceiveTask()
        {
            int bytesRead = 0;

            byte[] buff = new byte[1024];

            try
            {
                while ((bytesRead = _socket.Receive(buff)) > 0)
                {
                    string str = Encoding.ASCII.GetString(buff).Replace("\0", String.Empty).Trim();

                    str = str.Replace("\r", "");
                    str = parseTabs(str);
                    string[] lines = str.Split('\n');

                    lock (_blockMutex)
                    {
                        if (_block) continue;
                    }

                    lock (_rcvQMutex) 
                    {
                        for(int i = 0; i < lines.Length; i++)
                            _rcvQ.Enqueue(lines[i]);
                    }

                    buff = new byte[1024];
                }
            }
            catch (Exception e)
            {
                lock (_errorQMutex)
                {
                    _errorQ.Enqueue(e.Message);
                }
            }
        }

        private bool ProcessLocalCommand(string command)
        {
            switch (command)
            {
                case "/clear":
                    _console.MessageBuffer.Clear();
                    return false;
                case "/block":
                    lock(_blockMutex)
                        _block = !_block;
                    return false;
            }

            return true;
        }

        public void ToggleBlock()
        {
            lock (_blockMutex)
                _block = !_block;
        }

        public void Clear()
        {
            _console.MessageBuffer.Clear();
        }

        public void Send(string s)
        {
            try
            {
                _socket.SendTo(Encoding.ASCII.GetBytes(s + "\n"), _robotEP);
            }
            catch (Exception ex)
            {
                lock (_errorQMutex)
                {
                    _errorQ.Enqueue(ex.Message);
                }
            }
        }

        void ConsoleCommandSent(object sender, ConsoleMessageEventArgs e)
        {
            string message = e.Message.Text;

            if (ProcessLocalCommand(message))
                Send(message);
        }

        public void Update()
        {
            lock (_rcvQMutex)
            {
                while (_rcvQ.Count > 0)
                {
                    string message = _rcvQ.Dequeue();

                    _console.MessageBuffer.Add(new ConsoleMessage(message, Constants.CONSOLE_ROBOT));
                }
            }

            lock (_errorQMutex)
            {
                while (_errorQ.Count > 0)
                {
                    string message = _errorQ.Dequeue();

                    _console.MessageBuffer.Add(new ConsoleMessage(message, Constants.CONSOLE_ERROR));
                }
            }
        }
    }
}
