using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Dashboard.Library.Persistence
{
    public class FakeDataPersistence : IDataPersistence
    {
        private Dictionary<string, object> _fields;

        public FakeDataPersistence()
        {
            _fields = new Dictionary<string, object>();
        }

        private string generateTimeStamp()
        {
            return DateTime.Now.ToString("yyyy MM dd HH:mm:ss:ffff");
        }

        public void Set(string key, object val)
        {
            if (!val.GetType().IsSerializable && !(typeof(System.Runtime.Serialization.ISerializable).IsAssignableFrom(val.GetType())))
                throw new InvalidOperationException("A serializable type is required.");

            _fields[key] = val;
        }

        public T Get<T>(string key)
            where T : new()
        {
            if (!typeof(T).IsSerializable && !(typeof(System.Runtime.Serialization.ISerializable).IsAssignableFrom(typeof(T))))
                throw new InvalidOperationException("A serializable type is required.");

            if (!_fields.ContainsKey(key))
                return default(T);

            return (T)_fields[key];
        }

        public void Flush(string file)
        {
            // null
        }
    }
}
