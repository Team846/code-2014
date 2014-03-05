using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Dashboard
{
    public class NetConsoleControl : TomShane.Neoforce.Controls.Console
    {
        TomShane.Neoforce.Controls.Button optionsBtn;
        TomShane.Neoforce.Controls.Window optionsWindow;

        TomShane.Neoforce.Controls.Button cmd_reboot, cmd_clear, cmd_block;

        NetConsoleProvider _ncp;

        public NetConsoleProvider NetConsoleProvider
        {
            get { return _ncp; }
            set { _ncp = value; }
        }

        public NetConsoleControl(TomShane.Neoforce.Controls.Manager m)
            : base(m)
        {
        }

        protected override void PositionExtraControls()
        {
            if (optionsBtn == null)
            {
                optionsBtn = new TomShane.Neoforce.Controls.Button(Manager);
                optionsBtn.Init();
                optionsBtn.Click += new TomShane.Neoforce.Controls.EventHandler(optionsBtn_Click);

                optionsWindow = new TomShane.Neoforce.Controls.Window(Manager);
                optionsWindow.Init();
                optionsWindow.Text = "Quick Commands";
                optionsWindow.Resizable = false;
                optionsWindow.Height = 150;
                optionsWindow.Width = 185;
                optionsWindow.Hide();

                cmd_reboot = new TomShane.Neoforce.Controls.Button(Manager);
                cmd_reboot.Init();
                cmd_reboot.Click += new TomShane.Neoforce.Controls.EventHandler(reboot_Click);
                cmd_clear = new TomShane.Neoforce.Controls.Button(Manager);
                cmd_clear.Init();
                cmd_clear.Click += new TomShane.Neoforce.Controls.EventHandler(clear_Click);
                cmd_block = new TomShane.Neoforce.Controls.Button(Manager);
                cmd_block.Init();
                cmd_block.Click += new TomShane.Neoforce.Controls.EventHandler(block_Click);

                optionsWindow.Add(cmd_block);
                optionsWindow.Add(cmd_clear);
                optionsWindow.Add(cmd_reboot);

                cmd_reboot.Height = cmd_clear.Height = cmd_block.Height = optionsWindow.ClientHeight / 3;
                cmd_reboot.Width = cmd_clear.Width = cmd_block.Width = optionsWindow.ClientWidth;

                Add(optionsBtn, false);
                Add(optionsWindow, false);
            }

            int btnWidth = (int)(txtMain.Width * 0.1f) + 2;

            txtMain.Width = txtMain.Width - btnWidth;

            optionsBtn.Left = txtMain.Left + txtMain.Width + 2;
            optionsBtn.Top = txtMain.Top;
            optionsBtn.Text = "CMDs";

            cmd_reboot.SetPosition(0, 2 * optionsWindow.ClientHeight / 3);
            cmd_reboot.Text = "Reboot";

            cmd_block.SetPosition(0, optionsWindow.ClientHeight / 3);
            cmd_block.Text = "Block Output";

            cmd_clear.SetPosition(0, 0);
            cmd_clear.Text = "Clear";
        }

        void optionsBtn_Click(object sender, TomShane.Neoforce.Controls.EventArgs e)
        {
            if (optionsWindow.Visible) return;

            optionsWindow.Show();
        }

        void reboot_Click(object sender, TomShane.Neoforce.Controls.EventArgs e)
        {
            _ncp.Send("reboot");
        }

        void clear_Click(object sender, TomShane.Neoforce.Controls.EventArgs e)
        {
            _ncp.Clear();
        }

        void block_Click(object sender, TomShane.Neoforce.Controls.EventArgs e)
        {
            _ncp.ToggleBlock();
            cmd_block.Text = cmd_block.Text.Equals("Unblock Output") ? "Block Output" : "Unblock Output";
        }
    }
}
