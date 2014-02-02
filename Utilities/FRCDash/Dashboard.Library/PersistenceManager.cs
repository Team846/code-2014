using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Dashboard.Library.Persistence;

namespace Dashboard.Library
{
    public class PersistenceManager
    {
        private static IDataPersistence _persistence;

        public static IDataPersistence Persistence
        {
            get { return _persistence; }
            set { _persistence = value; }
        }

        public static void Start(string directory)
        {
            _persistence = new FakeDataPersistence();
        }
    }
}
