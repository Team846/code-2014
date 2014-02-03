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
            foreach (KeyValuePair<string, IPersistable> kvp in _fields)
            {
                FileStream file = File.Create(Path.Combine(dirname, kvp.Key + ".fdb"));

                kvp.Value.Serialize(file);

                file.Close();
            }
        }
    }
}
