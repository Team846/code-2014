using System;



using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dashboard.Library;
using TomShane.Neoforce.Controls;
using Dashboard.Library.Persistence;
using Dashboard.Library;
using Microsoft.Xna.Framework;
using RhesusNet.NET;
using System.IO;

namespace LRT14
{
    public class LogControl : DashboardControl, IPersistable
    {
        private string id;
        private string persistenceKey;
        private ContentLibrary content;

        Dictionary<string, KeyValuePair<byte, Dictionary<float, string>>> _msgs;
        byte currmax;

        TomShane.Neoforce.Controls.Console _console;

        public LogControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
            this.id = id;
            this.persistenceKey = persistenceKey;
            this.content = content;

            PersistenceManager.Persistence.Set(persistenceKey, this);

            this.Add(_console = new TomShane.Neoforce.Controls.Console(manager));

            currmax = 0;
            _msgs = new Dictionary<string, KeyValuePair<byte, Dictionary<float, string>>>();
        }

        public void Clear()
        {
            _msgs.Clear();
        }

        public void SetConsoleDimensions()
        {
            _console.Top = _console.Left = 0;
            _console.Height = this.Height;
            _console.Width = this.Width;
            _console.TextColor = Microsoft.Xna.Framework.Color.Yellow;
            _console.Parent = this;
        }

        public override void UpdateControl(GameTime gameTime)
        {
            NetBuffer nb = ReadMessage();

            while (nb != null)
            {
                float time = nb.ReadFloat();
                string category = nb.ReadString();
                string msg = nb.ReadString();

                if (!_msgs.ContainsKey(category))
                {
                    _msgs.Add(category, new KeyValuePair<byte, Dictionary<float, string>>(currmax, new Dictionary<float, string>()));
                    _console.Channels.Add(new ConsoleChannel(currmax++, category, Microsoft.Xna.Framework.Color.Yellow));
                }
                    
                _msgs[category].Value.Add(time, msg);
                _console.MessageBuffer.Add(new ConsoleMessage(time + " - [" + category + "] " + msg + "\n", _msgs[category].Key));

                nb = ReadMessage();
            }

            Invalidate();

            base.UpdateControl(gameTime);
        }

        public void Serialize(Stream stream)
        {
            throw new NotImplementedException();
        }

        public void Deserialize(Stream stream)
        {
            throw new NotImplementedException();
        }

        public string CSVOut()
        {
            string ret = "";
            foreach (KeyValuePair<string, KeyValuePair<byte, Dictionary<float, string>>> kvp in _msgs)
            {
                ret += kvp.Key + "," + kvp.Value.Key + "," + kvp.Value.Value + "\n";
            }

            return ret;
        }
    }
}
