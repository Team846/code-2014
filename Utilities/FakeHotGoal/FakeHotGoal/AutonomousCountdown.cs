using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FakeHotGoal
{
    public partial class AutonomousCountdown : Form
    {
        public AutonomousCountdown()
        {
            InitializeComponent();
        }

        private void AutonomousCountdown_Load(object sender, EventArgs e)
        {

        }

        public void SetCounter(int i)
        {
            countdownLabel.Text = i.ToString();
        }
    }
}
