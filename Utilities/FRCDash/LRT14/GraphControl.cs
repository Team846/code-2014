using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;
using STDConsole = System.Console;

using RhesusNet.NET;

using Dashboard.Library;
using Dashboard.Library.Persistence;

namespace LRT14
{
    class GraphControl : DashboardControl
    {

        Graph GlobalGraph;

        public GraphControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {

        }

        public override void Init()
        {
            //this.Height = ClientHeight;
            //this.Width = ClientWidth;

            Graph graph = new Graph(Manager, "Test Graph", "none", Content);
            graph.Height = this.Height;
            graph.Width = this.Width;
            graph.Parent = this;
            graph.Init();

            GlobalGraph = graph;



            //graph.Refresh();

            //_initialized = false;

            base.Init();
        }

        public override void UpdateControl(GameTime gameTime)
        {
            GlobalGraph.AddDataPoint(new Vector2((float)gameTime.TotalGameTime.TotalSeconds, (float)gameTime.TotalGameTime.TotalSeconds * (float)Math.Abs(Math.Sin((float)gameTime.TotalGameTime.TotalSeconds))));

            Invalidate();

            base.UpdateControl(gameTime);
        }
    }
}
