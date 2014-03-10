﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;

using RhesusNet.NET;

namespace Dashboard.Library
{
    public class NetworkedGraph : Graph
    {
        public NetworkedGraph(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
        }

        public override void UpdateControl(GameTime gameTime)
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
            {
                float remoteTime = nb.ReadFloat();
                float value = nb.ReadFloat();
                AddDataPoint(new Vector2(remoteTime, value));
            }

            base.UpdateControl(gameTime);
        }
    }
}