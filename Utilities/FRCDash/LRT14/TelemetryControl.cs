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
    public class TelemetryControl : DashboardControl
    {
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

        private int _topPadding;
        private int _leftPadding;
        private int _topMargin;
        private int _sideMargin;
        private int _textBoxHeight;
        private int _textBoxWidth;
        private int _labelInfoDistance;
        //private String[] _labels;
        //private short[] _idInfos;
        private Dictionary<short, FieldDatatype> _idDatatype; 
        private Dictionary<short, string> _idLabel;
        private Dictionary<short, string> _idData;

        private string id;
        private string persistenceKey;
        private ContentLibrary content;
      
        public TelemetryControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
            this.id = id;
            this.persistenceKey = persistenceKey;
            this.content = content;

            Color = new Color(30, 30, 30);
        }

        public void display()
        {
            int i = 0;
            foreach(KeyValuePair<short, string> kvp in _idLabel)
            {
                Label label = new Label(Manager);
                label.Init();
                label.Text = kvp.Value;
                label.Left = _leftPadding;
                label.Top = _topPadding + _topMargin * i + _textBoxHeight * i;
                label.Parent = this;


                TextBox info = new TextBox(Manager);
                info.Init();
                //info.Text = IdInfos[i].ToString();
                if (_idData.ContainsKey(kvp.Key))
                {
                    info.Text = _idData[kvp.Key];
                }
                else
                {
                    info.Text = "<???>";
                }


                info.Left = label.Left + _labelInfoDistance;
                info.Top = label.Top;
                info.Parent = this;

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

        public void telem_init()
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
            {

                short field = nb.ReadInt16();

                for (int i = 0; i < field; i++)
                {
                    string label = nb.ReadString();
                    short id = nb.ReadInt16();
                    byte datatype = nb.ReadByte();

                    _idData.Add(id, null);
                    _idDatatype.Add(id, (FieldDatatype)datatype);
                    _idLabel.Add(id, label);
                }
            }
        }

        public void telem_update()
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
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
                                data= nb.ReadByte().ToString();
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
        }
    }
}
