using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

using TomShane.Neoforce.Controls;

using RhesusNet.NET;

namespace Dashboard.Library
{
    public abstract class DashboardControl : Control
    {
        private string _id;
        private string _persistenceKey;
        private ContentLibrary _content;

        protected ContentLibrary Content
        {
            get { return _content; }
        }

        protected string PersistenceKey
        {
            get { return _persistenceKey; }
        }

        public DashboardControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager)
        {
            _id = id;
            _persistenceKey = persistenceKey;
            _content = content;
        }

        protected override void Update(GameTime gameTime)
        {
            UpdateControl(gameTime);

            base.Update(gameTime);
        }

        public void SubscribeToPacket(byte header)
        {
            NetworkManager.RegisterComponent(_id, header);
        }

        public NetBuffer ReadMessage()
        {
            return NetworkManager.ReadMessage(_id);
        }

        public virtual void UpdateControl(GameTime gameTime)
        {
            
        }
    }
}
