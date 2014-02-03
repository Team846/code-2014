using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Dashboard.Library.Persistence
{
    public interface IPersistable
    {
        void Serialize(Stream stream);
        void Deserialize(Stream stream);
    }
}
