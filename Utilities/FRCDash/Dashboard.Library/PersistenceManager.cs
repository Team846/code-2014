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

        private static int _lastSave;
        private static string _directory;

        public static IDataPersistence Persistence
        {
            get { return _persistence; }
        }

        public static void Start(string directory)
        {
            _persistence = new DataPersistence();
            _lastSave = Environment.TickCount;
            _directory = directory;
        }

        public static void Update()
        {
            if (Environment.TickCount - _lastSave > (1000 * 60)) // auto flush every minute
            {
                Persistence.Flush("dashboard_" + System.DateTime.Now.ToString("yyyy_MM_dd_HH.mm.ss.ffff") + ".sdb", true);
                _lastSave = Environment.TickCount;
            }
        }

        public static void Flush(string name)
        {
            _lastSave = Environment.TickCount;
            Persistence.Flush(name);
        }
    }
}
