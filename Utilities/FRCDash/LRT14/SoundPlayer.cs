using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework.Audio;

using Dashboard.Library;

using RhesusNet.NET;

using TomShane.Neoforce.Controls;

namespace LRT14
{
    public class SoundPlayer : DashboardControl
    { 
        private enum Events : byte
        {
            SHOOT = 0x00,
            COLLECT = 0x01,
            PURGE = 0x02,
            LAUNCHER_LOADED = 0x03
        }

        SoundEffect _shootSound;
        SoundEffect _collectSound;

        public SoundPlayer(Manager manager, string id, string persistenceKey, ContentLibrary content)
                : base(manager, id, persistenceKey, content)
        {
            _shootSound = (SoundEffect)content.Get("dashboard_shoot");
            _collectSound = (SoundEffect)content.Get("dashboard_collect");

            SoundEffect.MasterVolume = 1.0f;
        }

        public override void UpdateControl(Microsoft.Xna.Framework.GameTime gameTime)
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
            {
                Events eventType = (Events)nb.ReadByte();

                switch (eventType)
                {
                    case Events.COLLECT:
                        _collectSound.Play(1.0f, 0.0f, 0.0f);
                        break;
                    case Events.SHOOT:
                        _shootSound.Play(1.0f, 0.0f, 0.0f);
                        break;
                }
            }

            base.UpdateControl(gameTime);
        }

    }
}
