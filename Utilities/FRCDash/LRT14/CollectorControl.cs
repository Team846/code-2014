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
    class CollectorControl : DashboardControl
    {

        private int _width;
        private int _height;
        private Dictionary<short, FieldDatatype> _idDatatype;
        private Dictionary<short, string> _idLabel;
        private Dictionary<short, string> _idData;

        private enum FieldDatatype : byte
        {
            INT8 = 0x00,
            INT16 = 0x01,
            INT32 = 0x02,
            INT64 = 0x03,
            UINT8 = 0x04,
            UINT16 = 0x05,
            UINT32 = 0x06,
            UINT64 = 0x07,
            FLOAT = 0x08,
            DOUBLE = 0x09,
            STRING = 0x0A,
        }

        private enum CollectHeader : byte
        {
            COLLECT_INIT = 0x02,
            COLLECT_UPDATE = 0x03
        }

        public CollectorControl(Manager manager, string id, string persistenceKey, ContentLibrary content )
            : base(manager, id, persistenceKey, content)
        {
            
        }

        public void display()
        {
            Graph speedGraph = new Graph(Manager, "collector_speed", "LRT14.AerialAssist.COLLECTOR_SPEED", Content);
            speedGraph.Parent = this;
            speedGraph.Init();
        }

        public void collectInit(NetBuffer nb)
        {
            _idData = new Dictionary<short, string>();
            _idDatatype = new Dictionary<short, FieldDatatype>();
            _idLabel = new Dictionary<short, string>();
            while ((nb = ReadMessage()) != null)
            {
                float t = nb.ReadFloat();
                double x = nb.ReadFloat();
                double y = nb.ReadFloat();
                double theta = nb.ReadFloat();

                //AddDataPoint(t, x, y, theta);
            }
        }
        private void collectUpdate(NetBuffer nb)
        {
            short field = nb.ReadInt16();

            for (int i = 0; i < field; i++)
            {
                short id = nb.ReadInt16();
                string data = "???";

                if (_idDatatype.ContainsKey(id))
                {
                    switch (_idDatatype[id])
                    {
                        case FieldDatatype.INT8:
                            data = nb.ReadSByte().ToString();
                            break;
                        case FieldDatatype.INT16:
                            data = nb.ReadInt16().ToString();
                            break;
                        case FieldDatatype.INT32:
                            data = nb.ReadInt32().ToString();
                            break;
                        case FieldDatatype.INT64:
                            data = nb.ReadInt64().ToString();
                            break;
                        case FieldDatatype.UINT8:
                            data = nb.ReadByte().ToString();
                            break;
                        case FieldDatatype.UINT16:
                            data = nb.ReadUInt16().ToString();
                            break;
                        case FieldDatatype.UINT32:
                            data = nb.ReadUInt32().ToString();
                            break;
                        case FieldDatatype.UINT64:
                            data = nb.ReadUInt64().ToString();
                            break;
                        case FieldDatatype.FLOAT:
                            data = nb.ReadFloat().ToString();
                            break;
                        case FieldDatatype.DOUBLE:
                            data = nb.ReadDouble().ToString();
                            break;
                        case FieldDatatype.STRING:
                            data = nb.ReadString();
                            break;

                    }
                }

                _idData[id] = data;

            }
        }
        public int Width
        {
            get { return _width; }
            set { _width = value; }
        }

        public int Height
        {
            get { return _height; }
            set { _height = value; }
        }

        public override void UpdateControl(GameTime gameTime)
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
            {
                CollectHeader header = (CollectHeader)nb.ReadByte();

                switch (header)
                {
                    case CollectHeader.COLLECT_INIT:
                        collectInit(nb);
                        break;
                    case CollectHeader.COLLECT_UPDATE:
                        collectUpdate(nb);
                        break;
                }
            }

            base.UpdateControl(gameTime);
        }
    }
}
