using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Dashboard
{
    class Logger
    {
        private String m_filePath;
        private Dictionary<String, Object> m_fields;
        private int m_count;
        private StreamWriter m_file;

        public Logger(String filePath, String className,  Dictionary<String, Object> fields)
        {
            m_filePath = filePath + className + "_" + generateTimeStamp() + ".csv";

            m_file = new System.IO.StreamWriter(m_filePath, true);

            String firstLine = "Count,";

            m_count = 0;

            m_fields = fields;

            foreach (String s in m_fields.Keys)
            {
                firstLine += s + ",";
            }

            m_file.WriteLine(firstLine.Remove(firstLine.Length - 1));
        }

        private String generateTimeStamp()
        {
            return new DateTime().ToString("yyyyMMddHHmmssffff");
        }

        public void Log()
        {
            String line = ++m_count + ",";
            foreach(Object o in m_fields)
            {
                line = o.ToString() + ",";
            }
            line.Remove(line.Length - 1);

            m_file.WriteLine(line);
        }

    }
}
