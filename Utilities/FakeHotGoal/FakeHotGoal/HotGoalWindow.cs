﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using RhesusNet.NET;

namespace FakeHotGoal
{
    public partial class HotGoalWindow : Form
    {
        private const byte HOT_GOAL_HEADER = 0x00;

        HotGoal _hotGoal;
        NetClient _client;

        Thread _updateThread;

        public HotGoalWindow()
        {
            _hotGoal = HotGoal.NONE;
            _client = new NetClient();

            _updateThread = new Thread(UpdateTask);
            _updateThread.IsBackground = false;
            _updateThread.Start();

            InitializeComponent();
        }

        public void UpdateTask()
        {
            bool lastVal = _client.Connected;

            while (true)
            {
                if (_client.Connected && !lastVal)
                {
                    statusStrip1.Invoke((MethodInvoker)(() => statusLabel.Text = "Connected."));
                }
                else if (!_client.Connected && lastVal)
                {
                    statusStrip1.Invoke((MethodInvoker)(() => statusLabel.Text = "Disconnected."));
                }

                lastVal = _client.Connected;

                Thread.Sleep(1000);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void SendState()
        {
            if (!_client.Connected) return;

            NetBuffer nb = new NetBuffer(2);

            nb.Write(HOT_GOAL_HEADER);
            nb.Write((byte)_hotGoal);

            _client.Send(nb, NetChannel.NET_RELIABLE_SEQUENCED, 1);
        }

        private void hotgoalLeft_Click(object sender, EventArgs e)
        {
            if (_hotGoal == HotGoal.LEFT)
            {
                hotgoalLeft.FlatAppearance.BorderColor = Color.Black;
                _hotGoal = HotGoal.NONE;

                SendState();
                
                return;
            }

            if (_hotGoal == HotGoal.RIGHT) hotgoalRight.FlatAppearance.BorderColor = Color.Black;

            hotgoalLeft.FlatAppearance.BorderColor = Color.Yellow;

            _hotGoal = HotGoal.LEFT;

            SendState();
        }

        private void hotgoalRight_Click(object sender, EventArgs e)
        {
            if (_hotGoal == HotGoal.RIGHT)
            {
                hotgoalRight.FlatAppearance.BorderColor = Color.Black;
                _hotGoal = HotGoal.NONE;

                SendState();

                return;
            }

            if (_hotGoal == HotGoal.LEFT) hotgoalLeft.FlatAppearance.BorderColor = Color.Black;

            hotgoalRight.FlatAppearance.BorderColor = Color.Yellow;

            _hotGoal = HotGoal.RIGHT;

            SendState();
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_client.Connected) return;

            _client.Open();
            _client.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 846));

            statusLabel.Text = "Connecting...";
        }

        private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!_client.Connected) return;

            _client.Close();
            _client.Disconnect();

            statusLabel.Text = "Disconnected.";

            disconnectToolStripMenuItem.Enabled = false;
            connectToolStripMenuItem.Enabled = true;
        }
    }
}