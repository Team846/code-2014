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
    class TelemetryControl : DashboardControl
    {
        private int topPadding;
        private int leftPadding;
        private int topMargin;
        private int sideMargin;
        private int textBoxHeight;
        private int textBoxWidth;
        private int labelInfoDistance;
        private String[] labels;

        private string id;
        private string persistenceKey;
        private ContentLibrary content;
      
        public TelemetryControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
            this.id = id;
            this.persistenceKey = persistenceKey;
            this.content = content;

            Color = Color.Transparent;
            BackColor = Color.Aqua;
        }

        public void display()
        {


            for (int i = 0; i < labels.Length; i++)
            {
                Label label = new Label(Manager);
                //label.Text = labels[i];
                label.Text = "hi";
                label.Left = leftPadding;
                label.Top = topPadding + topMargin * i + textBoxHeight * i;
                //label.Color = Color.Red;
                label.Parent = this;
                label.Init();


                Label info = new Label(Manager);
                info.Text = "data";
                info.Left = label.Left + labelInfoDistance;
                info.Top = label.Top;
                info.Parent = this;
                info.Init();

                
            }
            
            
        }

        public int topPaddingMod
        {
            get { return topPadding; }
            set { topPadding = value; }
        }

        public int leftPaddingMod
        {
            get { return leftPadding;}
            set { leftPadding = value;}
        }

        public int topMarginMod
        {
            get { return topMargin; }
            set { topMargin = value; }
        }

        public int sideMarginMod
        {
            get { return sideMargin; }
            set { sideMargin = value; }
        }

        public int textBoxHeightMod
        {
            get { return textBoxHeight; }
            set { textBoxHeight = value; }
        }

        public int textBoxWidthMod
        {
            get { return textBoxHeight; }
            set { textBoxHeight = value; }
        }

        public String[] labelsMod
        {
            get { return labels; }
            set{labels = value;}
        }

        public int labelInfoDistanceMod
        {
            get{return labelInfoDistance;}
            set{labelInfoDistance = value;}
        }



    }


    class teleData
    {

        public string getData(string id)
        {


            return null;

        }

        
    }

}
