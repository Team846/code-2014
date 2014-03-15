namespace FakeHotGoal
{
    partial class HotGoalWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HotGoalWindow));
            this.hotgoalLeft = new System.Windows.Forms.Button();
            this.hotgoalRight = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.networkToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.disconnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.autonomousToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // hotgoalLeft
            // 
            this.hotgoalLeft.BackColor = System.Drawing.Color.White;
            this.hotgoalLeft.FlatAppearance.BorderColor = System.Drawing.Color.Black;
            this.hotgoalLeft.FlatAppearance.BorderSize = 5;
            this.hotgoalLeft.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.hotgoalLeft.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.hotgoalLeft.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.hotgoalLeft.Location = new System.Drawing.Point(12, 44);
            this.hotgoalLeft.Name = "hotgoalLeft";
            this.hotgoalLeft.Size = new System.Drawing.Size(256, 84);
            this.hotgoalLeft.TabIndex = 0;
            this.hotgoalLeft.Text = "L";
            this.hotgoalLeft.UseVisualStyleBackColor = false;
            this.hotgoalLeft.Click += new System.EventHandler(this.hotgoalLeft_Click);
            // 
            // hotgoalRight
            // 
            this.hotgoalRight.BackColor = System.Drawing.Color.White;
            this.hotgoalRight.FlatAppearance.BorderColor = System.Drawing.Color.Black;
            this.hotgoalRight.FlatAppearance.BorderSize = 5;
            this.hotgoalRight.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.hotgoalRight.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.hotgoalRight.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.hotgoalRight.Location = new System.Drawing.Point(277, 44);
            this.hotgoalRight.Name = "hotgoalRight";
            this.hotgoalRight.Size = new System.Drawing.Size(256, 84);
            this.hotgoalRight.TabIndex = 1;
            this.hotgoalRight.Text = "R";
            this.hotgoalRight.UseVisualStyleBackColor = false;
            this.hotgoalRight.Click += new System.EventHandler(this.hotgoalRight_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.networkToolStripMenuItem,
            this.autonomousToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(545, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // networkToolStripMenuItem
            // 
            this.networkToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.disconnectToolStripMenuItem});
            this.networkToolStripMenuItem.Name = "networkToolStripMenuItem";
            this.networkToolStripMenuItem.Size = new System.Drawing.Size(64, 20);
            this.networkToolStripMenuItem.Text = "Network";
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.connectToolStripMenuItem.Text = "Connect";
            this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
            // 
            // disconnectToolStripMenuItem
            // 
            this.disconnectToolStripMenuItem.Enabled = false;
            this.disconnectToolStripMenuItem.Name = "disconnectToolStripMenuItem";
            this.disconnectToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.disconnectToolStripMenuItem.Text = "Disconnect";
            this.disconnectToolStripMenuItem.Click += new System.EventHandler(this.disconnectToolStripMenuItem_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.BackColor = System.Drawing.Color.LightGray;
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 136);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(545, 22);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // statusLabel
            // 
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(82, 17);
            this.statusLabel.Text = "Disconnected.";
            // 
            // autonomousToolStripMenuItem
            // 
            this.autonomousToolStripMenuItem.Name = "autonomousToolStripMenuItem";
            this.autonomousToolStripMenuItem.Size = new System.Drawing.Size(89, 20);
            this.autonomousToolStripMenuItem.Text = "Autonomous";
            this.autonomousToolStripMenuItem.Click += new System.EventHandler(this.autonomousToolStripMenuItem_Click);
            // 
            // HotGoalWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(545, 158);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.hotgoalRight);
            this.Controls.Add(this.hotgoalLeft);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "HotGoalWindow";
            this.Text = "Hot Goal";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button hotgoalLeft;
        private System.Windows.Forms.Button hotgoalRight;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem networkToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem disconnectToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
        private System.Windows.Forms.ToolStripMenuItem autonomousToolStripMenuItem;
    }
}

