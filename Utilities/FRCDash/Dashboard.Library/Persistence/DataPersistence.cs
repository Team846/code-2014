using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

using RhesusNet.NET;

namespace Dashboard.Library.Persistence
{
    public class DataPersistence : IDataPersistence
    {
        private Dictionary<string, IPersistable> _fields;

        public DataPersistence()
        {
            _fields = new Dictionary<string, IPersistable>();
        }

        private string generateTimeStamp()
        {
            return DateTime.Now.ToString("yyyy MM dd HH:mm:ss:ffff");
        }

        public void Set(string key, IPersistable val)
        {
            _fields[key] = val;
        }

        public T Get<T>(string key)
            where T : IPersistable
        {
            if (!typeof(T).IsSerializable && !(typeof(System.Runtime.Serialization.ISerializable).IsAssignableFrom(typeof(T))))
                throw new InvalidOperationException("A serializable type is required.");

            if (!_fields.ContainsKey(key))
                return default(T);

            return (T)_fields[key];
        }

        public void Flush(string dirname)
        {
            if (!Directory.Exists(dirname))
                Directory.CreateDirectory(dirname);

            FileStream header = File.Create(Path.Combine(dirname, "header.fdbh"));
            StreamWriter sw = new StreamWriter(header);

            foreach (KeyValuePair<string, IPersistable> kvp in _fields)
            {
                FileStream file = File.Create(Path.Combine(dirname, kvp.Key + ".fdb"));

                sw.WriteLine(kvp.Key + "," + kvp.Value.GetType());    

                kvp.Value.Serialize(file);

                file.Close();
            }

            sw.Close();
        }

        public void Read(string dirname)
        {
            FileStream header = File.OpenRead(Path.Combine(dirname, "header.fdbh"));
            StreamReader sr = new StreamReader(header);

            string line;
            while ((line = sr.ReadLine()) != null)
            {
                string[] s = line.Split(',');
                _fields.Add(s[0], (IPersistable)Activator.CreateInstance(null, s[1]));
            }
            
            sr.Close();

            foreach (KeyValuePair<string, IPersistable> kvp in _fields)
            {
                FileStream file = File.OpenRead(Path.Combine(dirname, kvp.Key + ".fdb"));

                kvp.Value.Deserialize(file);

                file.Close();
            }
        }
    }
}
