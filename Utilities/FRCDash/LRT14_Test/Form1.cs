using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using RhesusNet.NET;

namespace LRT14_Test
{
    public partial class LRT14Tests : Form
    {
        NetServer _server;

        public LRT14Tests()
        {
            InitializeComponent();

            _server = new NetServer(1140);
            _server.Open();

            Messages.SetServer(_server);
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void collectButton_Click(object sender, EventArgs e)
        {
            Messages.Actions.SendCollect();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Messages.Actions.SendShoot();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            Messages.Actions.SendLauncherLoaded();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Messages.Actions.SendPurge();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            float drivetrainLeft = 0.0f;
            float drivetrainRight = 0.0f;

            Single.TryParse(textBox1.Text, out drivetrainLeft);
            Single.TryParse(textBox2.Text, out drivetrainRight);

            Messages.Actions.SendTelemetry(drivetrainLeft, drivetrainRight);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Messages.Actions.SendTelemetryInit();
        }

        private void logSend_Click(object sender, EventArgs e)
        {
            string tag = logTag.Text;
            string message = logMessage.Text;

            Messages.Actions.SendLog(tag, message);
        }
    }
}
