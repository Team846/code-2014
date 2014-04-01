using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;

using RhesusNet.NET;

using Dashboard.Library;
using Dashboard.Library.Persistence;

namespace LRT14
{
    public class RobotLog : DashboardControl
    {
        ListView _listView;

        public RobotLog(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {

        }

        public override void Init()
        {
            base.Init();

            _listView = new ListView(Manager, new string[] { "Tag", "Time", "Message" }, new float[] { 0.1f, 0.1f, 0.8f });
            _listView.Left = 0;
            _listView.Top = 0;
            _listView.Width = ClientWidth;
            _listView.Height = ClientHeight;
            _listView.Init();
            _listView.Parent = this;
        }

        public override void UpdateControl(GameTime gameTime)
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
            {
                string tag = nb.ReadString();
                float time = nb.ReadFloat();
                string message = nb.ReadString();

                message = message.Replace("\n", "  ");
                _listView.Items.Add(new string[] { tag, "+" + time + "s", message });
            }

            base.UpdateControl(gameTime);
        }
    }
}
