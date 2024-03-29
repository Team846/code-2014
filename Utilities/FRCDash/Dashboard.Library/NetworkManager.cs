﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;

using RhesusNet.NET;

namespace Dashboard.Library
{
    public class NetworkManager
    {
        private static NetClient netconn = new NetClient();
        private static NetBuffer b;

        private static Dictionary<string, Queue<NetBuffer>> _idToBufferList = new Dictionary<string, Queue<NetBuffer>>();
        private static Dictionary<byte, List<string>> _headerToIDMap = new Dictionary<byte, List<string>>();

        private static object _mapMutex = new object();

        private static bool _connected = true;

        private static string _ip;
        private static int _port;

        public static bool Connected
        {
            get { return _connected; }
        }

        public static void Start(string ip, int port)
        {
            _ip = ip;
            _port = port;
            netconn.Open();
            netconn.Connect(new IPEndPoint(IPAddress.Parse(ip), port));
        }

        public static void Disconnect()
        {
            _connected = false;
            netconn.Disconnect();
            netconn.Close();
        }

        public static void Reconnect()
        {
            _connected = true;
            netconn = new NetClient();
            Start(_ip, _port);
        }

        public static void UpdateNetwork()
        {
            if (!netconn.Connected) return;

            while (BufferState())
            {
                byte header = b.ReadByte();

                if (!AssertKeyExists(header))
                {
                    continue;
                    throw new KeyNotFoundException("Unknown message type received.");
                }

                if (!AssertBufferKeyExists(_headerToIDMap[header].ToArray()))
                {
                    continue;
                    throw new KeyNotFoundException("Internal message type mapping is not synchronized.");
                }

                lock (_mapMutex)
                {
                    for (int i = 0; i < _headerToIDMap[header].Count; i++)
                    {
                        _idToBufferList[_headerToIDMap[header].ElementAt(i)].Enqueue(b);
                    }
                }
            }
        }

        private static bool BufferState()
        {
            b = netconn.ReadMessage();
            return b != null;
        }

        private static bool AssertKeyExists(byte b)
        {
            return _headerToIDMap.ContainsKey(b);
        }

        private static bool AssertBufferKeyExists(params string[] keys)
        {
            for (int i = 0; i < keys.Count(); i++)
            {
                if (!_idToBufferList.ContainsKey(keys[i])) return false;
            }

            return true;
        }

        public static void RegisterComponent(string ID, byte header)
        {
            if (_idToBufferList.ContainsKey(ID))
                throw new ArgumentException("The specified ID already belongs to a component!");

            _idToBufferList.Add(ID, new Queue<NetBuffer>());
            if (!_headerToIDMap.ContainsKey(header))
            {
                _headerToIDMap.Add(header, new List<string>());
                _headerToIDMap[header].Add(ID);
            }
            else
            {
                _headerToIDMap[header].Add(ID);
            }
        }

        public static NetBuffer ReadMessage(string ID)
        {
            if (!_connected)
                return null;

            lock (_mapMutex)
            {
                if (!AssertBufferKeyExists(ID))
                    throw new KeyNotFoundException("Key not found: " + ID);

                return _idToBufferList[ID].Count > 0 ? _idToBufferList[ID].Dequeue() : null;
            }
        }

        public static void Send(NetBuffer nb, NetChannel method, int channel)
        {
            netconn.Send(nb, method, channel);
        }

        public static void Clear()
        {
            _idToBufferList.Clear();
            _headerToIDMap.Clear();
        }
    }
}
