using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;

using Dashboard.Library;
using Dashboard.Layouts;

namespace Dashboard
{
    public class Dashboard : Application
    {
        private ContentLibrary _contentLibrary;
        private NetConsoleProvider _ncp;

        private NetConsoleControl _console;
        private TabControl _tabControl;
        private MainMenu _menuStrip;

        private bool _clearNextUpdate;

        public Dashboard()
            : base("Simple", false)
        {
            windowSmall();

            Content.RootDirectory = "Content";

            // Setting up the shared skins directory
            //Manager.SkinDirectory = @"..\..\Skins\";

            ClearBackground = true;
            BackgroundColor = new Color(50, 50, 50);
            ExitConfirmation = false;

            Manager.AutoUnfocus = false;
            ExitConfirmation = true;

            _contentLibrary = new ContentLibrary();

            Manager.TargetFrames = 60;

            PersistenceManager.Start("data");
        }

        

        protected override void Initialize()
        {
            base.Initialize();

            ((System.Windows.Forms.Form)System.Windows.Forms.Control.FromHandle(this.Window.Handle)).Location = new System.Drawing.Point(0, 0);

            ResetLayout();
        }

        void edit_resetLayoutButton_Click(object sender, EventArgs e)
        {
            ResetLayout();
        }

        private void ResetLayout()
        {
            Manager.RemoveAll();

            NetworkManager.Clear();

            _menuStrip = new MainMenu(Manager);
            _menuStrip.Init();
            _menuStrip.Width = GraphicsDevice.Viewport.Width;

            MenuItem fileButton = new MenuItem("File");
            MenuItem file_loadlayoutButton = new MenuItem("Load Layout...");
            file_loadlayoutButton.Click += new EventHandler(file_loadlayoutButton_Click);
            fileButton.Items.Add(file_loadlayoutButton);
            MenuItem editButton = new MenuItem("Edit");
            MenuItem edit_resetLayoutButton = new MenuItem("Reset Layout...");
            edit_resetLayoutButton.Click += new EventHandler(edit_resetLayoutButton_Click);
            editButton.Items.Add(edit_resetLayoutButton);
            MenuItem debugButton = new MenuItem("Debug");
            MenuItem debug_flushPersistence = new MenuItem("Flush data buffer...");
            debug_flushPersistence.Click += new EventHandler(debug_flushPersistence_Click);
            debugButton.Items.Add(debug_flushPersistence);
            //MenuItem debug_load = new MenuItem("Load Log");
            //debug_load.Click += new EventHandler(debug_load_Click);
            //debugButton.Items.Add(debug_load);
            //MenuItem debug_unload = new MenuItem("Unload Log");
            //debug_unload.Click += new EventHandler(debug_unload_Click);
            //debugButton.Items.Add(debug_unload);
            //MenuItem debug_fstep = new MenuItem("Step Forward");
            //debug_fstep.Click += new EventHandler(debug_fstep_Click);
            //debugButton.Items.Add(debug_fstep);
            //MenuItem debug_bstep = new MenuItem("Step Backward");
            //debug_fstep.Click += new EventHandler(debug_bstep_Click);
            //debugButton.Items.Add(debug_bstep);

            MenuItem output = new MenuItem("Network");
            MenuItem disconnect = new MenuItem("Reconnect");
            disconnect.Click += new EventHandler(disconnect_click);
            output.Items.Add(disconnect);

            MenuItem window = new MenuItem("Window");
            MenuItem window_small = new MenuItem("Small");
            window_small.Click += new EventHandler(window_small_Click);
            MenuItem window_expanded = new MenuItem("Expanded");
            window_expanded.Click += new EventHandler(window_expanded_Click);
            window.Items.Add(window_small);
            window.Items.Add(window_expanded);
            //debugButton.Items.Add(window);

            _menuStrip.Items.Add(fileButton);
            _menuStrip.Items.Add(editButton);
            _menuStrip.Items.Add(debugButton);
            _menuStrip.Items.Add(output);

            Manager.Add(_menuStrip);

            _tabControl = new TabControl(Manager);
            _tabControl.Init();

            _tabControl.Top = _menuStrip.Height;
            _tabControl.Name = "DB_2.0_TAB_ROOT";
            _tabControl.Width = GraphicsDevice.Viewport.Width;
            _tabControl.Height = GraphicsDevice.Viewport.Height - _menuStrip.Height;
            _tabControl.Visible = true;

            TabPage console = _tabControl.AddPage();
            console.Init();
            console.Text = "Console";

            if (_console == null)
            {
                _console = new NetConsoleControl(Manager);
                _console.Init();
                _console.AddToConsoleOnSend = false;
                _console.Width = console.ClientWidth;
                _console.Height = console.ClientHeight;
                _console.Channels.Add(new ConsoleChannel(Constants.CONSOLE_ROBOT, "Robot", Color.Yellow));
                _console.Channels.Add(new ConsoleChannel(Constants.CONSOLE_LOCAL, "Local", Color.White));
                _console.Channels.Add(new ConsoleChannel(Constants.CONSOLE_ERROR, "Error", Color.Red));
                _console.SelectedChannel = 0;
                _console.ChannelsVisible = true;

                _ncp = new NetConsoleProvider(_console, "10.8.46.2");
                _ncp.Start();

                _console.NetConsoleProvider = _ncp;

                NetworkManager.Start("10.8.46.2", 1140);
            }

            _console.Parent = console;

            Manager.Add(_tabControl);
        }

        void debug_load_Click(object sender, EventArgs e)
        {
            // CHANGEME
        }

        void debug_unload_Click(object sender, EventArgs e)
        {
            // CHANGEME
        }

        void debug_fstep_Click(object sender, EventArgs e)
        {
            // CHANGEME
        }

        void debug_bstep_Click(object sender, EventArgs e)
        {
            // CHANGEME
        }

        void window_expanded_Click(object sender, EventArgs e)
        {
            windowExpanded();
        }

        void window_small_Click(object sender, EventArgs e)
        {
            windowSmall();
        }

        void windowExpanded()
        {
            Manager.Graphics.PreferredBackBufferWidth = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
            Manager.Graphics.PreferredBackBufferHeight = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height - 300;

            Manager.Graphics.ApplyChanges();
        }

        void windowSmall()
        {
            Manager.Graphics.PreferredBackBufferWidth = 800;
            Manager.Graphics.PreferredBackBufferHeight = 600;

            Manager.Graphics.ApplyChanges();
        }

        void debug_flushPersistence_Click(object sender, EventArgs e)
        {
            PersistenceManager.Persistence.Flush("dashboard_" + System.DateTime.Now.ToString("yyyy_MM_dd_HH.mm.ss.ffff") + ".sdb");
        }

        void file_loadlayoutButton_Click(object sender, EventArgs e)
        {
            // for debugging purposes
            ResetLayout();

            ILayoutLoader loader = new CompiledLayoutLoader(System.IO.Path.GetFullPath("LRT14.dll"));
            loader.LoadLayout(Manager, _contentLibrary);

            return;

            FileDialog fileDialog = new FileDialog(Manager);
            fileDialog.Init();
            fileDialog.Text = "Test";
            fileDialog.Left = 10;
            fileDialog.Top = 10;
            fileDialog.StayOnTop = true;
            fileDialog.Closing += new WindowClosingEventHandler(fileDialog_Closing);
            Manager.Add(fileDialog);
        }

        void fileDialog_Closing(object sender, WindowClosingEventArgs e)
        {
            FileDialog dialog = (FileDialog)sender;

            ModalResult result = dialog.ModalResult;

            if (result == ModalResult.Ok)
            {
                string file = dialog.FileName;

                ResetLayout();

                ILayoutLoader loader = new CompiledLayoutLoader(file);
                loader.LoadLayout(Manager, _contentLibrary);
            }
        }

        void disconnect_click(object sender, EventArgs e)
        {
            if (NetworkManager.Connected)
                NetworkManager.Disconnect();
            else
                NetworkManager.Reconnect(); 

            ((MenuItem)sender).Text = NetworkManager.Connected ? "Disconnect" : "Connect";
        }

        protected override void LoadContent()
        {
            base.LoadContent();

            Content.RootDirectory = "Content";

            _contentLibrary.Load(Content, GraphicsDevice);
        }

        protected override void Update(GameTime gameTime)
        {
            NetworkManager.UpdateNetwork();

            _ncp.Update();

            base.Update(gameTime);
        }
    }
}