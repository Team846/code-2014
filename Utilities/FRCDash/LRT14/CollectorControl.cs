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
    public class CollectorControl : DashboardControl
    {
        private int _topPadding;
        private int _leftPadding;
        private int _topMargin;
        private int _sideMargin;
        private int _textBoxHeight;
        private int _textBoxWidth;
        private int _labelInfoDistance;

        public int TopPadding
        {
            get { return _topPadding; }
            set { _topPadding = value; }
        }

        public int LeftPadding
        {
            get { return _leftPadding; }
            set { _leftPadding = value; }
        }

        public int TopMargin
        {
            get { return _topMargin; }
            set { _topMargin = value; }
        }

        public int SideMargin
        {
            get { return _sideMargin; }
            set { _sideMargin = value; }
        }

        public int TextBoxHeight
        {
            get { return _textBoxHeight; }
            set { _textBoxHeight = value; }
        }

        public int TextBoxWidth
        {
            get { return _textBoxHeight; }
            set { _textBoxHeight = value; }
        }

        public int LabelInfoDistance
        {
            get { return _labelInfoDistance; }
            set { _labelInfoDistance = value; }
        }


        private int _width;
        private int _height;
        //private Dictionary<short, FieldDatatype> _idDatatype;
        //private Dictionary<short, string> _idLabel;
        private Dictionary<short, DataField> _idData;
        private Dictionary<short, TextBox> _idTextbox;
        private bool _initialized;

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

        private class DataField
        {
            string _data;
            FieldDatatype _dataType;
            string _label;
            bool _graph;

            public DataField(string data, FieldDatatype type, string label, bool graph)
            {
                _data = data;
                _dataType = type;
                _label = label;
                _graph = graph;
            }

            public DataField(string data, FieldDatatype type, string label)
            {
                _data = data;
                _dataType = type;
                _label = label;
                _graph = false;
            }

            public string Data
            {
                get { return _data; }
                set { _data = value; }
            }

            public string Label
            {
                get { return _label; }
                set { _label = value; }
            }

            public FieldDatatype DataType
            {
                get { return _dataType; }
                set { _dataType = value; }
            }

            public bool IsGraphed
            {
                get { return _graph; }
                set { _graph = value; }
            }
        }

        private enum CollectHeader : byte
        {
            COLLECT_INIT = 0x02,
            COLLECT_UPDATE = 0x03
        }

        public CollectorControl(Manager manager, string id, string persistenceKey, ContentLibrary content )
            : base(manager, id, persistenceKey, content)
        {
            _initialized = false;
            Color = Color.Transparent;
        }

        public void display()
        {
            NetworkedGraph speedGraph = new NetworkedGraph(Manager, "collector_speed", "LRT14.AerialAssist.COLLECTOR_SPEED", Content);
            speedGraph.Init();
            speedGraph.Parent = this;
            speedGraph.SubscribeToPacket((byte)0x02); //TODO: Fix dat shit
            //speedGraph.AddDataPoint(new Vector2(1,2));

            int i = 0;
            foreach(KeyValuePair<short, DataField> kvp in _idData)
            {
                Label label = new Label(Manager);
                label.Init();
                label.Text = kvp.Value.Label;
                label.Left = _leftPadding;
                label.Top = _topPadding + _topMargin * i + _textBoxHeight * i;
                label.Parent = this;

                TextBox info = new TextBox(Manager);
                _idTextbox[kvp.Key] = info;
                info.ReadOnly = true;
                info.Init();
                //info.Text = IdInfos[i].ToString();

                //TODO: implement graphing

                info.Text = kvp.Value.Data != null ? kvp.Value.Data : "<???>";

                info.Left = label.Left + _labelInfoDistance;
                info.Top = label.Top;
                info.Parent = this;

                i++;
            }  
            
        }

        public void collectInit(NetBuffer nb)
        {
            //_idData = new Dictionary<short, string>();
            //_idDatatype = new Dictionary<short, FieldDatatype>();
            //_idLabel = new Dictionary<short, string>();

            int field = nb.ReadInt16();
            _idData = new Dictionary<short, DataField>();
            _idTextbox = new Dictionary<short, TextBox>();

            for (int i = 0; i < field; i++)
            {
                string label = nb.ReadString();
                short id = nb.ReadInt16();
                byte datatype = nb.ReadByte();
                bool graph = datatype == (byte)FieldDatatype.STRING ? false : nb.ReadBool();

                _idData.Add(id, new DataField("", (FieldDatatype)datatype, label, graph));
            }
            _initialized = true;
            display();
        }
        private void collectUpdate(NetBuffer nb)
        {
            if (!_initialized)
                return;

            short field = nb.ReadInt16();

            for (int i = 0; i < field; i++)
            {
                short id = nb.ReadInt16();
                string data = "???";

                if (_idData.ContainsKey(id))
                {
                    switch (_idData[id].DataType)
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

                //_idData[id] = data;

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