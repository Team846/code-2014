using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;

using RhesusNet.NET;

namespace MockLRT14
{
    class Program
    {
        enum MessageType : byte
        {
            LOCATOR = 0x0,
            TELEMETRY = 0x01,
            COLLECTOR_SPEED = 0x02,
            COLLECTOR = 0x03
        }

        enum TelemHeader : byte
        {
            TELEM_INIT = 0x00,
            TELEM_UPDATE = 0x01
        }

        private enum FieldDatatype : byte
        {
            INT8 = 0x00,
            INT16 = 0x01,
            INT32 = 0x02,
            INT64 = 0x03,
            UINT8 = 0x04,
            UINT16 = 0x05,
            UINT32 = 0x06,
            UINT64 = 0x07,
            FLOAT = 0x08,
            DOUBLE = 0x09,
            STRING = 0x0A,
        }

        enum UnreliableSequencedChannel : int
        {
            LOCATOR = 0,
            TELEM = 1,
        }

        static void Main(string[] args)
        {
            NetServer server = new NetServer(846);

            server.Open();

            Console.WriteLine("Server listening on port 846");

            Random r = new Random();

            Stopwatch sw = new Stopwatch();

            float totalTime = 0f;

            Thread.Sleep(10000);

            NetBuffer telemInit = new NetBuffer();
            telemInit.Write((byte)MessageType.TELEMETRY);
            telemInit.Write((byte)TelemHeader.TELEM_INIT);
            telemInit.Write((short)11);

            telemInit.Write("foo");
            telemInit.Write((short)0);
            telemInit.Write((byte)FieldDatatype.INT8);
            telemInit.Write(true);

            telemInit.Write("bar");
            telemInit.Write((short)1);
            telemInit.Write((byte)FieldDatatype.INT16);
            telemInit.Write(false);

            telemInit.Write("baz");
            telemInit.Write((short)2);
            telemInit.Write((byte)FieldDatatype.INT32);
            telemInit.Write(false);
            
            telemInit.Write("qux");
            telemInit.Write((short)3);
            telemInit.Write((byte)FieldDatatype.INT64);
            telemInit.Write(false);
            
            telemInit.Write("quux");
            telemInit.Write((short)4);
            telemInit.Write((byte)FieldDatatype.UINT8);
            telemInit.Write(false);
            
            telemInit.Write("corge");
            telemInit.Write((short)5);
            telemInit.Write((byte)FieldDatatype.UINT16);
            telemInit.Write(false);
            
            telemInit.Write("grault");
            telemInit.Write((short)6);
            telemInit.Write((byte)FieldDatatype.UINT32);
            telemInit.Write(false);
            
            telemInit.Write("garply");
            telemInit.Write((short)7);
            telemInit.Write((byte)FieldDatatype.UINT64);
            telemInit.Write(false);
            
            telemInit.Write("waldo");
            telemInit.Write((short)8);
            telemInit.Write((byte)FieldDatatype.FLOAT);
            telemInit.Write(false);
            
            telemInit.Write("fred");
            telemInit.Write((short)9);
            telemInit.Write((byte)FieldDatatype.DOUBLE);
            telemInit.Write(false);
            
            telemInit.Write("plugh");
            telemInit.Write((short)10);
            telemInit.Write((byte)FieldDatatype.STRING);
            telemInit.Write(false);

            server.SendToAll(telemInit, NetChannel.NET_UNRELIABLE_SEQUENCED, (int)UnreliableSequencedChannel.TELEM);

            while (true)
            {
                NetBuffer buff = new NetBuffer();

                totalTime += (float)sw.Elapsed.TotalSeconds;
                //value = (float)r.NextDouble();

                sw.Restart();

                // x^2 + y^2 = r^2
                // sin^2(t) + cos^2(t) = r^2
                //
                // Parametrization:
                // x = 3cos(t)
                // y = 3sin(t)
                //
                //  dx
                // ---- = -3sin(t)
                //  dt
                //
                //  dy
                // ---- = 3cos(t)
                //  dt

                double x = 12 * Math.Cos(totalTime);
                double y = 12 * Math.Sin(totalTime);

                double dXdT = 12 * -Math.Sin(totalTime);
                double dYdT = 12 * Math.Cos(totalTime);

                double angle = Math.Atan2(dYdT, dXdT) * 180 / Math.PI;

                buff.Write((byte)MessageType.LOCATOR);
                buff.Write((float)totalTime);
                buff.Write((float)x);
                buff.Write((float)y);
                buff.Write((float)angle);

                Console.WriteLine("X: {0}, Y: {1}, Theta: {2}", x, y, angle);

                server.SendToAll(buff, NetChannel.NET_UNRELIABLE_SEQUENCED, (int)UnreliableSequencedChannel.LOCATOR);

                NetBuffer nb = new NetBuffer();
                nb.Write((byte)MessageType.TELEMETRY);
                nb.Write((byte)TelemHeader.TELEM_UPDATE);
                nb.Write((short)11);
                nb.Write((short)0);
                nb.Write(sbyte.MinValue);
                nb.Write((short)1);
                nb.Write(short.MinValue);
                nb.Write((short)2);
                nb.Write(int.MinValue);
                nb.Write((short)3);
                nb.Write(long.MinValue);
                nb.Write((short)4);
                nb.Write(byte.MaxValue);
                nb.Write((short)5);
                nb.Write(ushort.MaxValue);
                nb.Write((short)6);
                nb.Write(uint.MaxValue);
                nb.Write((short)7);
                nb.Write(ulong.MaxValue);
                nb.Write((short)8);
                nb.Write(3.14f);
                nb.Write((short)9);
                nb.Write(double.MaxValue);
                nb.Write((short)10);
                nb.Write("Thank You BasedGod!");

                server.SendToAll(nb, NetChannel.NET_UNRELIABLE_SEQUENCED, (int)UnreliableSequencedChannel.TELEM);

                Thread.Sleep(50);
            }
        }
    }
}
