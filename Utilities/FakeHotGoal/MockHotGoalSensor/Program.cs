using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

using RhesusNet.NET;

namespace MockHotGoalSensor
{
    class Program
    {
        static void Main(string[] args)
        {
            NetServer server = new NetServer(846);

            server.Open();

            Console.WriteLine("Server listening on port 846");

            Random r = new Random();

            float totalTime = 0f;

            Thread.Sleep(1000);

            while (true)
            {
                NetBuffer buff;

                while ((buff = server.ReadMessage()) != null)
                {
                    byte header = buff.ReadByte();

                    if (header == 0x00) // HOT_GOAL_HEADER
                    {
                        byte side = buff.ReadByte();

                        string text = "???";

                        if (side == 0)
                        {
                            text = "HotGoal.LEFT";
                        }
                        else if (side == 1)
                        {
                            text = "HotGoal.RIGHT";
                        }
                        else if (side == 2)
                        {
                            text = "HotGoal.NONE";
                        }

                        Console.WriteLine("Hot Goal state changed: {0}", text);
                    }
                }

                Thread.Sleep(100);
            }
        }
    }
}
