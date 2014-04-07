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
            if (!_fields.ContainsKey(key))
                return default(T);

            return (T)_fields[key];
        }

        public void Flush(string dirname)
        {
            Flush(dirname, false);
        }

        public void Flush(string dirname, bool clear)
        {
            if (!Directory.Exists(dirname))
                Directory.CreateDirectory(dirname);

            FileStream header = File.Create(Path.Combine(dirname, "header.fdbh"));
            StreamWriter sw = new StreamWriter(header);

            foreach (KeyValuePair<string, IPersistable> kvp in _fields)
            {
                FileStream file = File.Create(Path.Combine(dirname, kvp.Key + ".csv"));

                sw.WriteLine(kvp.Key + "," + kvp.Value.GetType());    

                //kvp.Value.Serialize(file);
                StreamWriter f = new StreamWriter(file);
                f.WriteLine(kvp.Value.CSVOut());

                f.Close();

                if (clear)
                    kvp.Value.Clear();
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
