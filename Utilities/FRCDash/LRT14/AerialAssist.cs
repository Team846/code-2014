using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

using TomShane.Neoforce.Controls;

using LayoutContract;

using Dashboard.Library;

namespace LRT14
{
    public class AerialAssist : ILayout
    {
        private enum MessageType : byte
        {
            LOCATOR = 0x0,
        }

        public void SetupLayout(Manager manager, ContentLibrary contentLibrary)
        {
            // Get our root control
            TabControl tabControl = (TabControl)manager.GetControl("DB_2.0_TAB_ROOT");

            setupLocator(manager, contentLibrary, tabControl);
        }

        private void setupLocator(Manager manager, ContentLibrary contentLibrary, TabControl root)
        {
            TabPage locatorTab = root.AddPageBeforeEnd();
            locatorTab.Init();
            locatorTab.Text = "Locator";

            RobotLocationControl locator = new RobotLocationControl(manager, "locator", "LRT14.AerialAssist.LOCATION", contentLibrary);
            locator.Top = 2;
            locator.Left = 2;
            locator.Width = locatorTab.ClientWidth - 4;
            locator.Height = locatorTab.ClientHeight - 4;
            locator.RobotWidthFeet = 2.46f;
            locator.RobotHeightFeet = 2.13f;
            locator.FieldWidthFeet = 24.67f;
            locator.FieldHeightFeet = 54;
            locator.FeetPerPixel = 0.1f;
            locator.BackgroundColor = Color.White;
            locator.FieldColor = Color.DarkGray;
            locator.RobotColor = Color.Red;
            locator.InitialPosition = new Vector2(locator.FieldWidthFeet / 2f, locator.FieldHeightFeet / 2f);
            locator.RelativePosition = Vector2.One;
            locator.Init();
            locator.SubscribeToPacket((byte)MessageType.LOCATOR);
            locator.Parent = locatorTab;
        }
    }
}
