using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dashboard.Library.Persistence;
using Microsoft.Xna.Framework;
using System.IO;
using RhesusNet.NET;
using System.Diagnostics.Contracts;

namespace LRT14
{
    public class RobotLocationData : IPersistable
    {
        private LinkedList<KeyValuePair<float, Vector3>> _data;

        private LinkedListNode<KeyValuePair<float, Vector3>> _currStep;

        public RobotLocationData()
        {
            _data = new LinkedList<KeyValuePair<float, Vector3>>();
            _currStep = _data.First;
        }

        public void ClearData()
        {
            _data.Clear();
            _currStep = _data.First;
        }

        public void AddData(float t, float x, float y, float theta)
        {
            _data.AddLast(new KeyValuePair<float, Vector3>(t, new Vector3(x, y, theta)));
        }

        public void Serialize(Stream stream)
        {
            int initCount = 4 + _data.Count * (4 + 3 * 4);
            NetBuffer buff = new NetBuffer(initCount); // count + count * (t, x, y, theta)

            buff.Write(_data.Count);

            foreach (KeyValuePair<float, Vector3> kvp in _data)
            {
                buff.Write(kvp.Key);
                buff.Write(kvp.Value.X);
                buff.Write(kvp.Value.Y);
                buff.Write(kvp.Value.Z);
            }

            Contract.Assert(buff.GetBufferLength() == initCount);

            stream.Write(buff.GetBuffer(), 0, buff.GetBufferLength());
        }

        public void Deserialize(Stream stream)
        {
            byte[] header = new byte[4];

            stream.Read(header, 0, 4);

            NetBuffer buff = new NetBuffer(header, header.Length);

            int length = buff.ReadInt32();

            int bufferLen = length * (4 + 3 * 4);

            byte[] byteBuff = new byte[bufferLen];
            stream.Read(byteBuff, 0, bufferLen);

            buff = new NetBuffer(byteBuff, bufferLen);

            for (int i = 0; i < length; i++)
            {
                float t = buff.ReadFloat();
                float x = buff.ReadFloat();
                float y = buff.ReadFloat();
                float theta = buff.ReadFloat();

                _data.AddLast(new KeyValuePair<float, Vector3>(t, new Vector3(x, y, theta)));
            }
        }

        public Vector3 GetValue()
        {
            return _currStep.Value.Value;
        }

        public string CSVOut()
        {
            string ret = "";
            foreach (KeyValuePair<float, Vector3> kvp in _data)
            {
                ret += kvp.Key + "," + kvp.Value.X + "," + kvp.Value.Y + "," + kvp.Value.Z + "\n";
            }

            return ret;
        }
    }
}
