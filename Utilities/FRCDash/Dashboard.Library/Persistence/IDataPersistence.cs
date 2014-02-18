using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Dashboard.Library.Persistence
{
    public interface IDataPersistence
    {
        void Set(string key, IPersistable val);

        T Get<T>(string key)
            where T : IPersistable;

        void Flush(string dirname);
        void Read(string dirname);
    }
}
