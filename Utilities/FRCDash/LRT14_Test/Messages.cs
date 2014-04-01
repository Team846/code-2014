using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using RhesusNet.NET;

namespace LRT14_Test
{
    public enum MessageType
    {
        LOCATOR = 0x0,
        COLLECTOR_SPEED = 0x2,
        TELEMETRY = 0x01,
        COLLECTOR = 0x03,
        EVENT_NOTIFICATION = 0x04,
        TELEMETRY_INIT_REQ = 0x05,
        LOG = 0x06
    }

    public static class Messages
    {
        private static NetServer _server;

        public static void SetServer(NetServer server)
        {
            _server = server;
        }

        public static class Actions
        {
            private enum Events : byte
            {
                SHOOT = 0x00,
                COLLECT = 0x01,
                PURGE = 0x02,
                LAUNCHER_LOADED = 0x03
            }

            public static void SendCollect()
            {
                NetBuffer nb = new NetBuffer();
                nb.Write((byte)MessageType.EVENT_NOTIFICATION);
                nb.Write((byte)Events.COLLECT);

                _server.SendToAll(nb, NetChannel.NET_RELIABLE_SEQUENCED, 1);
            }

            public static void SendShoot()
            {
                NetBuffer nb = new NetBuffer();
                nb.Write((byte)MessageType.EVENT_NOTIFICATION);
                nb.Write((byte)Events.SHOOT);
                _server.SendToAll(nb, NetChannel.NET_RELIABLE_SEQUENCED, 1);
            }

            public static void SendPurge()
            {
                NetBuffer nb = new NetBuffer();
                nb.Write((byte)MessageType.EVENT_NOTIFICATION);
                nb.Write((byte)Events.PURGE);
                _server.SendToAll(nb, NetChannel.NET_RELIABLE_SEQUENCED, 1);
            }

            public static void SendLauncherLoaded()
            {
                NetBuffer nb = new NetBuffer();
                nb.Write((byte)MessageType.EVENT_NOTIFICATION);
                nb.Write((byte)Events.LAUNCHER_LOADED);
                _server.SendToAll(nb, NetChannel.NET_RELIABLE_SEQUENCED, 1);
            }

            public static void SendTelemetryInit()
            {
                NetBuffer nb = new NetBuffer();
                nb.Write((byte)MessageType.TELEMETRY);
                nb.Write((byte)0x00);
                nb.Write((short)2);
                nb.Write("DT Left");
                nb.Write((short)0);
                nb.Write((byte)0x08);
                nb.Write(false);
                nb.Write("DT Right");
                nb.Write((short)1);
                nb.Write((byte)0x08);
                nb.Write(false);

                _server.SendToAll(nb, NetChannel.NET_UNRELIABLE_SEQUENCED, 2);
            }

            public static void SendTelemetry(float drivetrainLeft, float drivetrainRight)
            {
                NetBuffer nb = new NetBuffer();

                float seconds = Environment.TickCount / 1000.0f;

                nb.Write((byte)MessageType.TELEMETRY);
                nb.Write((byte)0x01);
                nb.Write(seconds);
                nb.Write((short)2);
                nb.Write((short)0);
                nb.Write(drivetrainLeft);
                nb.Write((short)1);
                nb.Write(drivetrainRight);

                _server.SendToAll(nb, NetChannel.NET_UNRELIABLE_SEQUENCED, 2);
            }

            public static void SendLog(string tag, string message)
            {
                NetBuffer nb = new NetBuffer();

                float seconds = Environment.TickCount / 1000.0f;

                nb.Write((byte)MessageType.LOG);
                nb.Write(tag);
                nb.Write(seconds);
                nb.Write(message);

                _server.SendToAll(nb, NetChannel.NET_UNRELIABLE_SEQUENCED, 3);
            }
        }
    }
}
