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
    public partial class SelectFieldsInRangeDialog : Form
    {
        public double ResultMin;
        public double ResultMax;
        public bool Success = false;

        public SelectFieldsInRangeDialog()
        {
            InitializeComponent();
            textBoxMin.Focus();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                ResultMin = Double.Parse(textBoxMin.Text);
                ResultMax = Double.Parse(textBoxMax.Text);
                Success = true;
            }
            catch
            {
                MessageBox.Show("Invalid input");
            }
        }
    }
}
