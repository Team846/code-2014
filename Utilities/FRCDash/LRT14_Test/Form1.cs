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
    }
}
