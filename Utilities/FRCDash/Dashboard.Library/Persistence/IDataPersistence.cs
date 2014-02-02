using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Dashboard.Library.Persistence
{
    public interface IDataPersistence
    {
        void Set(string key, object val);

        T Get<T>(string key)
            where T : new();

        void Flush(string file);
    }
}
