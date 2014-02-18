using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Dashboard.Library.Persistence
{
    public class FakeDataPersistence : IDataPersistence
    {
        private Dictionary<string, IPersistable> _fields;

        public FakeDataPersistence()
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
            return (T)_fields[key];
        }

        public void Flush(string dirname)
        {
            // null
        }

        public void Read(string dirname)
        {
            //null
        }
    }
}
