using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dashboard.Library;
using TomShane.Neoforce.Controls;
using RhesusNet.NET;
using Microsoft.Xna.Framework;
using System.IO;

namespace LRT14Analysis
{
    public class TelemetryAnalysisControl : AnalysisControl
    {
        private LinkedList<KeyValuePair<float, Dictionary<short, DataField>>> _buffer;

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

        private enum TelemHeader : byte
        {
            TELEM_INIT = 0x00,
            TELEM_UPDATE = 0x01
        }

        private class DataField
        {
            string _data;
            FieldDatatype _dataType;
            string _label;
            bool _graphFlag;
            Graph _graph;

            public DataField(string data, FieldDatatype type, string label, bool graph, Graph g)
            {
                _data = data;
                _dataType = type;
                _label = label;
                _graphFlag = graph;
                _graph = g;
            }

            public DataField(string data, FieldDatatype type, string label)
            {
                _data = data;
                _dataType = type;
                _label = label;
                _graphFlag = false;
                _graph = null;
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
                get { return _graphFlag; }
                set { _graphFlag = value; }
            }

            public Graph Graph
            {
                get { return _graph; }
                set { _graph = value; }
            }
        }

        private int _topPadding;
        private int _leftPadding;
        private int _topMargin;
        private int _sideMargin;
        private int _textBoxHeight;
        private int _textBoxWidth;
        private int _labelInfoDistance;
        //private String[] _labels;
        //private short[] _idInfos;
        private Dictionary<short, DataField> _idData;
        private Dictionary<short, TextBox> _idTextbox;

        private string id;
        private string persistenceKey;
        private ContentLibrary content;

        private bool _initialized;
      
        public TelemetryAnalysisControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
            this.id = id;
            this.persistenceKey = persistenceKey;
            this.content = content;

            Color = Microsoft.Xna.Framework.Color.Transparent;

            _buffer = new LinkedList<KeyValuePair<float, Dictionary<short, DataField>>>();

            _initialized = false;
        }

        public void display()
        {
            int i = 0;
            foreach(KeyValuePair<short, DataField> kvp in _idData)
            {
                Label label = new Label(Manager);
                label.Init();
                label.Text = kvp.Value.Label;
                label.Left = i % 2 == 0 ?_leftPadding : _leftPadding + this.ClientWidth / 2;
                label.Top = _topPadding + _topMargin * i + _textBoxHeight * i;
                label.Top -= i % 2 == 0 ? 0 : _textBoxHeight + _topMargin;
                label.Parent = this;

                TextBox info = new TextBox(Manager);
                _idTextbox[kvp.Key] = info;
                info.ReadOnly = true;
                info.Init();
                //info.Text = IdInfos[i].ToString();

                info.Text = kvp.Value.Data != null ? kvp.Value.Data : "<???>";

                info.Left = label.Left + _labelInfoDistance;
                info.Top = label.Top;
                info.Parent = this;

                if (kvp.Value.IsGraphed)
                {
                    this.Add(kvp.Value.Graph);
                    kvp.Value.Graph.SetPosition(300, i * TextBoxHeight);
                }

                i++;
            }  
        }

        public int TopPadding
        {
            get { return _topPadding; }
            set { _topPadding = value; }
        }

        public int LeftPadding
        {
            get { return _leftPadding;}
            set { _leftPadding = value;}
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
            get{return _labelInfoDistance;}
            set{_labelInfoDistance = value;}
        }

        private void telemInit(NetBuffer nb)
        {
            int field = nb.ReadInt16();
            _idData = new Dictionary<short, DataField>();
            _idTextbox = new Dictionary<short, TextBox>();

            for (int i = 0; i < field; i++)
            {
                string label = nb.ReadString();
                short id = nb.ReadInt16();
                byte datatype = nb.ReadByte();
                bool graph = datatype == (byte)FieldDatatype.STRING ? false : nb.ReadBool();

                _idData.Add(id, new DataField("", (FieldDatatype)datatype, label, graph, graph ? new Graph(Manager, label, label + id, this.Content) : null));
            }

            _initialized = true;

            display();
        }

        public override void UpdateControl(GameTime gameTime)
        {
            

            base.UpdateControl(gameTime);
        }

        public void Serialize(Stream stream)
        {
            NetBuffer buff = new NetBuffer();

            buff.Write(_idData.Count);
            foreach (KeyValuePair<float, Dictionary<short, DataField>> b in _buffer)
            {
                foreach (KeyValuePair<short, DataField> kvp in _idData)
                {
                    buff.Write(kvp.Value.Label);
                    buff.Write(kvp.Value.Data);
                }
            }

            stream.Write(buff.GetBuffer(), 0, buff.GetBufferLength());
        }

        public void Deserialize(Stream stream)
        {
                //byte[] header = new byte[4];

                //stream.Read(header, 0, 4);

                //NetBuffer buff = new NetBuffer(header, header.Length);

                //int length = buff.ReadInt32();

                //int bufferLen = length * (4 + 3 * 4);

                //byte[] byteBuff = new byte[bufferLen];
                //stream.Read(byteBuff, 0, bufferLen);

                //buff = new NetBuffer(byteBuff, bufferLen);

                //for (int i = 0; i < length; i++)
                //{
                //    float t = buff.ReadFloat();
                //    float x = buff.ReadFloat();
                //    float y = buff.ReadFloat();
                //    float theta = buff.ReadFloat();

                //    _data.Add(t, new Vector3(x, y, theta));
                //}
            //}
        }

        public void StepForward()
        {

        }

        public void StepBackward()
        {

        }
    }
}
