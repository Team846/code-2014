using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RobotCSVAnalyzer
{
    public partial class SelectFieldsWithDialog : Form
    {
        public string Result;

        public SelectFieldsWithDialog()
        {
            InitializeComponent();
            textBox1.Focus();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Result = textBox1.Text;
        }
    }
}
