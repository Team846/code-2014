using System;
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
        private const int FIVE_SECONDS = 5000;

        HotGoal _hotGoal;
        NetClient _client;

        AutonomousCountdown countdown;

        volatile bool _isAuton;
        volatile int _autonStart;

        Thread _updateThread;

        public HotGoalWindow()
        {
            _hotGoal = HotGoal.NONE;
            _isAuton = false;

            countdown = new AutonomousCountdown();

            _updateThread = new Thread(UpdateTask);
            _updateThread.IsBackground = false;
            _updateThread.Start();

            InitializeComponent();
        }

        public void UpdateTask()
        {
            bool lastVal = _client != null && _client.Connected;
            bool autonLastWasMoreThanFiveSeconds = false;

            while (true)
            {
                if (_client != null)
                {
                    if (_client.Connected && !lastVal)
                    {
                        statusStrip1.Invoke((MethodInvoker)(() => statusLabel.Text = "Connected."));
                        menuStrip1.Invoke((MethodInvoker)(() => connectToolStripMenuItem.Enabled = false));
                        menuStrip1.Invoke((MethodInvoker)(() => disconnectToolStripMenuItem.Enabled = true));
                    }
                    else if (!_client.Connected && lastVal)
                    {
                        statusStrip1.Invoke((MethodInvoker)(() => statusLabel.Text = "Disconnected."));
                        menuStrip1.Invoke((MethodInvoker)(() => connectToolStripMenuItem.Enabled = true));
                        menuStrip1.Invoke((MethodInvoker)(() => disconnectToolStripMenuItem.Enabled = false));
                    }

                    lastVal = _client.Connected;
                }

                if (_isAuton)
                {
                    int end = _autonStart + 2 * FIVE_SECONDS;

                    int millisLeft = end - Environment.TickCount;
                    if (millisLeft < 0) millisLeft = 0;

                    int secondsLeft = (millisLeft + 500) / 1000;

                    if (secondsLeft < 5 && !autonLastWasMoreThanFiveSeconds)
                    {
                        if (_hotGoal == HotGoal.LEFT) _hotGoal = HotGoal.RIGHT;
                        else if (_hotGoal == HotGoal.RIGHT) _hotGoal = HotGoal.LEFT;

                        SendState();

                        updateButtons();
                    }

                    if (secondsLeft <= 0)
                    {
                        _isAuton = false;
                        countdown.Invoke((MethodInvoker)(() => countdown.Hide()));
                        menuStrip1.Invoke((MethodInvoker)(() => autonomousToolStripMenuItem.Enabled = true ));
                    }

                    autonLastWasMoreThanFiveSeconds = secondsLeft < 5;

                    countdown.Invoke((MethodInvoker)(() => countdown.SetCounter(secondsLeft)));
                }
                else
                    autonLastWasMoreThanFiveSeconds = false;

                Thread.Sleep(100);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void SendState()
        {
            if (_client == null || !_client.Connected) return;

            NetBuffer nb = new NetBuffer(2);

            nb.Write(HOT_GOAL_HEADER);
            nb.Write((byte)_hotGoal);

            _client.Send(nb, NetChannel.NET_RELIABLE_SEQUENCED, 1);
        }

        private void updateButtons()
        {
            hotgoalLeft.FlatAppearance.BorderColor = Color.Black;
            hotgoalRight.FlatAppearance.BorderColor = Color.Black;

            if (_hotGoal == HotGoal.LEFT)
                hotgoalLeft.FlatAppearance.BorderColor = Color.Yellow;

            if (_hotGoal == HotGoal.RIGHT)
                hotgoalRight.FlatAppearance.BorderColor = Color.Yellow;
        }

        private void hotgoalLeft_Click(object sender, EventArgs e)
        {
            if (_isAuton) return;

            if (_hotGoal == HotGoal.LEFT)
            {
                _hotGoal = HotGoal.NONE;

                SendState();

                updateButtons();
                
                return;
            }

            _hotGoal = HotGoal.LEFT;

            SendState();

            updateButtons();
        }

        private void hotgoalRight_Click(object sender, EventArgs e)
        {
            if (_isAuton) return;

            if (_hotGoal == HotGoal.RIGHT)
            {
                _hotGoal = HotGoal.NONE;

                SendState();

                updateButtons();

                return;
            }

            _hotGoal = HotGoal.RIGHT;

            updateButtons();

            SendState();
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_client != null && _client.Connected) return;

            _client = new NetClient();

            _client.Open();
            _client.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 846));

            statusLabel.Text = "Connecting...";
        }

        private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_client != null && !_client.Connected) return;

            _updateThread.Abort();

            _client.Disconnect();
            _client.Close();

            statusLabel.Text = "Disconnected.";

            disconnectToolStripMenuItem.Enabled = false;
            connectToolStripMenuItem.Enabled = true;
        }
        
        private void autonomousToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_isAuton) return;

            autonomousToolStripMenuItem.Enabled = false;

            _autonStart = Environment.TickCount;
            _isAuton = true;

            Random rand = new Random();
            _hotGoal = (HotGoal)rand.Next(0, 2);

            SendState();

            updateButtons();
            
            countdown.SetCounter(10);
            countdown.Show();
        }
    }
}
