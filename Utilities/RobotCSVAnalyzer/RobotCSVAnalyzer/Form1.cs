using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Resources;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace RobotCSVAnalyzer
{
    public partial class mainWindow : Form
    {
        private const string settingsFile = "settings.conf";
        private Dictionary<string, string> settings;
        private Dictionary<string, List<double>> data;
        private List<string> fields;
        private List<string> sections;
        private Dictionary<string, string> fieldToSections;
        private int cycles = 0;
        bool propogatingCheckboxes = false;
        bool massChecking = false;
        private Dictionary<TreeNode, Series> legendMap;
        private Dictionary<string, TreeNode> fieldToNode;
        private string currentFile = null;
        DataTable table;
        private double offset = 0;
        private double startOffset = 0;
        private double min = 0;
        private double max = 0;
        private double scale = 1;
        private double startScale = 1;
        private Point mouseStart = Point.Empty;
        private DataPoint selectedPoint = null;
        private List<Series> selectedLines;

        public mainWindow()
        {
            InitializeComponent();
            Size = new Size(Screen.PrimaryScreen.WorkingArea.Size.Width - 100, Screen.PrimaryScreen.WorkingArea.Size.Height - 100);
            data = new Dictionary<string, List<double>>();
            fields = new List<string>();
            settings = new Dictionary<string, string>();
            sections = new List<string>();
            fieldToSections = new Dictionary<string, string>();
            legendMap = new Dictionary<TreeNode, Series>();
            fieldToNode = new Dictionary<string, TreeNode>();
            selectedLines = new List<Series>();
            settings["RobotIP"] = "10.8.46.2";
            settings["LogFile"] = "log.bin";
            settings["LogDecoderPath"] = "LogDecoder.exe";
            if (!System.IO.File.Exists(settingsFile))
            {
                System.IO.FileStream fs = System.IO.File.Create(settingsFile);
                fs.Close();
                using (System.IO.StreamWriter file = new System.IO.StreamWriter(settingsFile))
                {
                    foreach(KeyValuePair<string, string> entry in settings)
                    {
                        file.WriteLine(entry.Key + ": " + entry.Value);
                    }
                }
            }
            using (System.IO.StreamReader file = new System.IO.StreamReader(settingsFile))
            {
                string line;
                while ((line = file.ReadLine()) != null)
                {
                    settings[line.Substring(0, line.IndexOf(':'))] = line.Substring(line.IndexOf(':') + 1).Trim();
                }
                Console.WriteLine("Loaded settings:");
                foreach (KeyValuePair<string, string> entry in settings)
                {
                    Console.WriteLine(entry.Key + ": " + entry.Value);
                }
            }
            toolStripProgressBar1.Visible = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void loadCSVMenu_Click(object sender, EventArgs e)
        {
            Stream myStream = null;
            var openFileDialog1 = new OpenFileDialog();

            openFileDialog1.Filter = "CSV files (*.csv)|*.csv";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    if ((myStream = openFileDialog1.OpenFile()) != null)
                    {
                        using (myStream)
                        {
                            loadCSV(myStream);
                            currentFile = openFileDialog1.FileName;
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Could not open CSV file");
                }
            }
            setStatus("Successfully loaded CSV file");
        }

        private void downloadFromRobotMenu_Click(object sender, EventArgs e)
        {
            toolStripProgressBar1.Value = 0;
            toolStripProgressBar1.Visible = true;
            statusStrip.Refresh();
            string log, header;
            try
            {
                log = downloadFile(settings["RobotIP"], settings["LogFile"]);
                toolStripProgressBar1.Value = 25;
                statusStrip.Refresh();
                header = downloadFile(settings["RobotIP"], settings["LogFile"] + ".header");
                toolStripProgressBar1.Value = 50;
                statusStrip.Refresh();
            }
            catch (Exception ex)
            {
                setStatus("Could not download log file from robot");
                MessageBox.Show(ex.Message, "Could not download log file from robot");
                toolStripProgressBar1.Visible = false;
                return;
            }
            setStatus("Decoding downloaded log file to CSV...");
            if (!System.IO.File.Exists(settings["LogDecoderPath"]))
            {
                setStatus("Log file decoder not found in path: " + settings["LogDecoderPath"]);
                MessageBox.Show("Log file decoder not found in path: " + settings["LogDecoderPath"], "Log file decoder not found");
            }
            Process decoder = new Process();
            decoder.StartInfo.FileName = settings["LogDecoderPath"];
            decoder.StartInfo.Arguments = log;
            decoder.StartInfo.CreateNoWindow = true;
            decoder.StartInfo.UseShellExecute = false;
            decoder.Start();
            decoder.WaitForExit();
            if (!System.IO.Directory.Exists("Downloaded"))
            {
                System.IO.Directory.CreateDirectory("Downloaded");
            }
            string csv = log.Substring(0, log.LastIndexOf('.')) + ".csv";
            if (!System.IO.File.Exists(System.IO.Path.Combine(Path.GetTempPath() + "RobotCSVAnalyzer", csv)))
            {
                setStatus("Fatal error in log file decoder at: " + settings["LogDecoderPath"]);
                MessageBox.Show("No CSV file generated by log file decoder at: " + settings["LogDecoderPath"], "Fatal error in log file decoder");
                toolStripProgressBar1.Visible = false;
                return;
            }
            toolStripProgressBar1.Value = 75;
            statusStrip.Refresh();
            try
            {
                var extension = Path.GetExtension(Path.GetFileName(csv));
                var newName = Path.GetFileName(csv).Replace(extension, DateTime.Now.ToString("-yyyyMMddHHmmdd") + extension);
                var filename = System.IO.Path.Combine(Directory.GetCurrentDirectory() + "\\Downloaded", newName);
                System.IO.File.Copy(csv, filename, true);
                csv = filename;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                setStatus("Fatal error in log file decoder at: " + settings["LogDecoderPath"]);
                MessageBox.Show("Log file decoder did not exit properly: " + settings["LogDecoderPath"], "Fatal error in log file decoder");
                toolStripProgressBar1.Visible = false;
                return;
            }
            toolStripProgressBar1.Value = 100;
            statusStrip.Refresh();
            loadCSV(new FileStream(csv, FileMode.Open));
            setStatus("Successfully downloaded and decoded log file from robot");
            toolStripProgressBar1.Visible = false;
            currentFile = csv;
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void setStatus(string status)
        {
            toolStripStatusLabel1.Text = status + " ";
            statusStrip.Refresh();
        }

        private void loadCSV(Stream stream)
        {
            data.Clear();
            fieldToSections.Clear();
            fields.Clear();
            sections.Clear();
            table = new DataTable();
            using (StreamReader sr = new StreamReader(stream))
            {
                string row;
                bool header = true;
                cycles = 0;
                while ((row = sr.ReadLine()) != null)
                {
                    string[] values = row.Split(',');
                    int cnt = 0;
                    if (cycles > 0)
                        table.Rows.Add();
                    foreach (string s in values)
                    {
                        if (header)
                        {
                            data[s] = new List<double>();
                            fields.Add(s);
                            table.Columns.Add(s);
                            if (s != "Cycle")
                            {
                                if (!sections.Contains(s.Substring(0, s.IndexOf('/'))))
                                    sections.Add(s.Substring(0, s.IndexOf('/')));
                                fieldToSections[s] = s.Substring(0, s.IndexOf('/'));
                            }
                        }
                        else
                        {
                            double value = Double.Parse(s);
                            data[fields[cnt]].Add(value);
                            table.Rows[cycles - 1][cnt] = value;
                        }
                        cnt++;
                    }
                    cycles++;
                    header = false;
                }
            }
            min = data["Cycle"].Min();
            max = data["Cycle"].Max();
            table1.DataSource = table;
            chart1.Series.Clear();
            legend.Nodes.Clear();
            legendMap.Clear();
            foreach (string s in sections)
            {
                legend.Nodes.Add(new TreeNode(s));
            }
            foreach (KeyValuePair<string, List<double>> column in data)
            {
                if (column.Key == "Cycle")
                    continue;
                var series = new Series
                {
                    Name = column.Key,
                    IsVisibleInLegend = true,
                    IsXValueIndexed = true,
                    ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line,
                    XValueMember = "Cycle",
                    YValueMembers = column.Key
                };
                for (int i = 0; i < column.Value.Count; i++)
                {
                    series.Points.AddXY(data["Cycle"][i], column.Value[i]);
                }
                chart1.Series.Add(series);
                int index = legend.Nodes[sections.IndexOf(column.Key.Substring(0, column.Key.IndexOf('/')))].Nodes.Add(new TreeNode(column.Key.Substring(column.Key.IndexOf('/') + 1)));
                legendMap[legend.Nodes[sections.IndexOf(column.Key.Substring(0, column.Key.IndexOf('/')))].Nodes[index]] = series;
                fieldToNode[column.Key] = legend.Nodes[sections.IndexOf(column.Key.Substring(0, column.Key.IndexOf('/')))].Nodes[index];
            }
            chart1.ApplyPaletteColors();
            foreach (KeyValuePair<TreeNode, Series> k in legendMap)
            {
                k.Key.BackColor = k.Value.Color;
            }
            chart1.Palette = ChartColorPalette.None;
            foreach (TreeNode n in legend.Nodes)
            {
                n.Expand();
                n.Checked = true;
            }
            legend.Focus();
            legend.SelectedNode = null;
            foreach (Series s in selectedLines)
            {
                s.BorderWidth = 1;
            }
            selectedLines.Clear();
            openInExcelToolStripMenuItem.Enabled = true;
            chart1.ChartAreas["ChartArea1"].AxisX.ScaleView.ZoomReset();
            scale = 1;
            offset = 0;
        }

        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (propogatingCheckboxes)
                return;
            propogatingCheckboxes = true;
            checkNodes(e.Node, e.Node, e.Node.Checked);
            propogatingCheckboxes = false;
            if (!massChecking)
                chart1.ChartAreas["ChartArea1"].RecalculateAxesScale();
        }
        
        private void checkNodes(TreeNode node, TreeNode sender, bool check)
        {
            if (node.Nodes.Contains(sender))
            {
                bool success = true;
                if (check == false)
                {
                    foreach (TreeNode child in node.Nodes)
                    {
                        if (child.Checked != check)
                        {
                            success = false;
                            break;
                        }
                    }
                }
                if (success)
                {
                    if (node.Parent != null)
                        checkNodes(node.Parent, node, check);
                }
                else
                {
                    return;
                }
            }
            else if (sender == node.Parent)
            {
                foreach (TreeNode child in node.Nodes)
                {
                    checkNodes(child, node, check);
                }
            }
            else
            {
                foreach (TreeNode child in node.Nodes)
                {
                    checkNodes(child, node, check);
                }
                if (node.Parent != null)
                    checkNodes(node.Parent, node, check);
            }
            node.Checked = check;
            if (node.Nodes.Count == 0)
            {
                if (!node.Checked)
                {
                    legendMap[node].Enabled = false;
                }
                else
                {
                    legendMap[node].Enabled = true;
                }
            }
        }

        private string downloadFile(string server, string file, string username = "", string password = "")
        {
            string downloadedFile = "";
            string serverPath = "ftp://" + server + "/" + file;
            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(serverPath);
            request.Method = WebRequestMethods.Ftp.GetFileSize;
            FtpWebResponse respSize = (FtpWebResponse)request.GetResponse();
            respSize = (FtpWebResponse)request.GetResponse();
            long size = respSize.ContentLength;
            respSize.Close();
            request = (FtpWebRequest)WebRequest.Create(serverPath);
            request.KeepAlive = true;
            request.UsePassive = true;
            request.UseBinary = true;
            request.Timeout = 10000;
            request.Method = WebRequestMethods.Ftp.DownloadFile;
            request.Credentials = new NetworkCredential(username, password);
            setStatus("Downloading " + file + " from robot using IP: " + server);

            using (FtpWebResponse response = (FtpWebResponse)request.GetResponse())
            using (Stream responseStream = response.GetResponseStream())
            {
                if (!Directory.Exists(Path.GetTempPath() + "RobotCSVAnalyzer"))
                {
                    Directory.CreateDirectory(Path.GetTempPath() + "RobotCSVAnalyzer");
                }
                using (FileStream destination = File.Open(Path.GetTempPath() + "RobotCSVAnalyzer\\" + file, FileMode.Create))
                {
                    byte[] buffer = new byte[size];
                    while (true)
                    {
                        int read = responseStream.Read(buffer, 0, buffer.Length);
                        if (read <= 0)
                            break;
                        destination.Write(buffer, 0, read);
                    }
                    destination.Flush();
                    downloadedFile = Path.GetTempPath() + "RobotCSVAnalyzer\\" + file;
                    destination.Close();
                }
            }
            return downloadedFile;
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            foreach (TreeNode n in legend.Nodes)
            {
                n.Checked = true;
            }
        }

        private void deselectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            foreach (TreeNode n in legend.Nodes)
            {
                n.Checked = false;
            }
        }

        private void selectFieldsWithToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SelectFieldsWithDialog dialog = new SelectFieldsWithDialog();
            dialog.ShowDialog();
            if (dialog.DialogResult == DialogResult.OK)
            {
                string search = dialog.Result;
                dialog.Dispose();
                massChecking = true;
                foreach (KeyValuePair<string, List<double>> column in data)
                {
                    if (column.Key == "Cycle")
                        continue;
                    if (column.Key.Contains(search))
                        fieldToNode[column.Key].Checked = true;
                    else
                        fieldToNode[column.Key].Checked = false;
                }
                massChecking = false;
                chart1.ChartAreas["ChartArea1"].RecalculateAxesScale();
            }
        }

        private void selectFieldsInRangeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SelectFieldsInRangeDialog dialog = new SelectFieldsInRangeDialog();
            dialog.ShowDialog();
            if (dialog.DialogResult == DialogResult.OK && dialog.Success)
            {
                double rangeMin = dialog.ResultMin;
                double rangeMax = dialog.ResultMax;
                dialog.Dispose();
                massChecking = true;
                foreach (KeyValuePair<string, List<double>> column in data)
                {
                    if (column.Key == "Cycle")
                        continue;
                    if (column.Value.Max() <= rangeMax && column.Value.Min() >= rangeMin)
                        fieldToNode[column.Key].Checked = true;
                    else
                        fieldToNode[column.Key].Checked = false;
                }
                massChecking = false;
                chart1.ChartAreas["ChartArea1"].RecalculateAxesScale();
            }
        }

        private void chart1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.X > 0.083 * chart1.Width && e.X < 0.97 * chart1.Width && e.Y > 0.89 * chart1.Height && e.Y < 0.974 * chart1.Height || mouseStart != Point.Empty)
            {
                this.Cursor = new System.Windows.Forms.Cursor("cursor-hand.cur");
            }
            else
            {
                this.Cursor = Cursors.Default;
            }
            if (mouseStart != Point.Empty)
            {
                scale = startScale + (mouseStart.Y - e.Y) / 100.0;
                scale = Math.Max(scale, 1);
                scale = Math.Min(scale, (max - min) / 10.0);
                offset = startOffset - (e.X - mouseStart.X) / (chart1.Width - 400.0) * 1 / scale * (max - min);
                offset = Math.Max(offset, -(1 - 1 / scale) / 2 * (max - min));
                offset = Math.Min(offset, (1 - 1 / scale) / 2 * (max - min));
                chart1.ChartAreas["ChartArea1"].AxisX.ScaleView.Zoom(min + offset + (1 - 1 / (scale)) / 2 * (max - min), max + offset - (1 - 1 / (scale)) / 2 * (max - min));
            }
        }

        private void chart1_MouseLeave(object sender, EventArgs e)
        {
            this.Cursor = Cursors.Default;
        }

        private void chart1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.X > 130 && e.X < chart1.Width - 320 && e.Y > chart1.Height - 70 && e.Y < chart1.Height && e.Button == MouseButtons.Left)
            {
                mouseStart = e.Location;
                startOffset = offset;
                startScale = scale;
                return;
            }
            var result = chart1.HitTest(e.X, e.Y);
            if (result.ChartElementType == ChartElementType.DataPoint)
            {
                int i = result.PointIndex;
                DataPoint dp = result.Series.Points[i];
                setStatus(string.Format("{0}: ({1}, {2})", result.Series.Name, dp.XValue, dp.YValues[0]));
                toolTip1.Show(string.Format("{0}\n({1}, {2})", result.Series.Name, dp.XValue, dp.YValues[0]), chart1, e.X, e.Y);
                if (selectedPoint != null)
                {
                    //selectedPoint.MarkerStyle = MarkerStyle.None;
                }
                selectedPoint = dp;
                //selectedPoint.MarkerStyle = MarkerStyle.Diamond;
                legend.SelectedNode = fieldToNode[result.Series.Name];
            }
            else
            {
                toolTip1.Hide(chart1);
                setStatus("");
                if (selectedPoint != null)
                {
                    //selectedPoint.MarkerStyle = MarkerStyle.None;
                    selectedPoint = null;
                }
                foreach (Series s in selectedLines)
                {
                    s.BorderWidth = 1;
                }
                selectedLines.Clear();
                legend.SelectedNode = null;
            }
        }

        private void chart1_MouseUp(object sender, MouseEventArgs e)
        {
            mouseStart = Point.Empty;
            toolTip1.Hide(chart1);
        }

        private void openInExcelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (currentFile != null)
                System.Diagnostics.Process.Start(currentFile);
        }

        private void resetZoomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            chart1.ChartAreas["ChartArea1"].AxisX.ScaleView.ZoomReset();
            scale = 1;
            offset = 0;
        }

        private void legend_AfterSelect(object sender, TreeViewEventArgs e)
        {
            foreach (Series s in selectedLines)
            {
                s.BorderWidth = 1;
            }
            selectedLines.Clear();
            if (legendMap.ContainsKey(e.Node))
            {
                legendMap[e.Node].BorderWidth = 3;
                selectedLines.Add(legendMap[e.Node]);
            }
            else
            {
                foreach (TreeNode child in e.Node.Nodes)
                {
                    legendMap[child].BorderWidth = 3;
                    selectedLines.Add(legendMap[child]);
                }
            }
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControl1.SelectedTab == graphView)
                legend.Focus();
        }
    }
}
