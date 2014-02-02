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
            LOCATOR = 0x0
        }

        enum UnreliableSequencedChannel : int
        {
            LOCATOR = 0
        }

        static void Main(string[] args)
        {
            NetServer server = new NetServer(846);

            server.Open();

            Console.WriteLine("Server listening on port 846");

            Random r = new Random();

            Stopwatch sw = new Stopwatch();

            float totalTime = 0f;

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

                Thread.Sleep(50);
            }
        }
    }
}
