using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

using TomShane.Neoforce.Controls;

using LayoutContract;

using Dashboard.Library;
using Dashboard.Library.Persistence;

namespace LRT14
{
    public class AerialAssist : ILayout
    {
        private enum MessageType : byte
        {
            LOCATOR = 0x0,
            COLLECTOR_SPEED = 0x2,
            TELEMETRY = 0x01,
            COLLECTOR = 0x03
        }

        public void SetupLayout(Manager manager, ContentLibrary contentLibrary)
        {
            PersistenceManager.Persistence = new DataPersistence();

            // Get our root control
            TabControl tabControl = (TabControl)manager.GetControl("DB_2.0_TAB_ROOT");

            setupLocator(manager, contentLibrary, tabControl);
            setupCollector(manager, contentLibrary, tabControl);
            setupTelemetry(manager, contentLibrary, tabControl);
            setupGraph(manager, contentLibrary, tabControl);
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

        private void setupCollector(Manager manager, ContentLibrary contentLibrary, TabControl root)
        {
            TabPage collectorTab = root.AddPageBeforeEnd();
            collectorTab.Init();
            collectorTab.Text = "Collector";

            CollectorControl collector = new CollectorControl(manager, "collector", "LRT14.AerialAssist.COLLECTOR_SPEED", contentLibrary);        
            collector.Top = 2;
            collector.Left = 2;
            collector.Width = collectorTab.ClientWidth - 4;
            collector.Height = collectorTab.ClientHeight - 4;
            collector.TopPadding = 10;
            collector.LeftPadding = collectorTab.ClientWidth-100;
            collector.TopMargin = 10;
            collector.SideMargin = 10;
            collector.TextBoxHeight = 10;
            collector.TextBoxWidth = 20;
            collector.LabelInfoDistance = 50;
            collector.Init();
            collector.SubscribeToPacket((byte)MessageType.COLLECTOR);
            collector.Parent = collectorTab;
            collectorTab.Add(collector);
        }

        private void setupTelemetry(Manager manager, ContentLibrary contentLibrary, TabControl root)
        {

            TabPage telemetryTab = root.AddPageBeforeEnd();
            telemetryTab.Text = "Telemetry";
            telemetryTab.Init();

            TelemetryControl telemetry = new TelemetryControl(manager, "telemetry", "LRT14.AerialAssist.TELEMETRY", contentLibrary);
            telemetry.Top = 2;
            telemetry.Left = 2;
            telemetry.Width = telemetryTab.ClientWidth - 4;
            telemetry.Height = telemetryTab.ClientHeight - 4;
            telemetry.TopPadding = 10;
            telemetry.LeftPadding = 10;
            telemetry.TopMargin = 10;
            telemetry.SideMargin = 10;
            telemetry.TextBoxHeight = 10;
            telemetry.TextBoxWidth = 20;
            telemetry.LabelInfoDistance = 50;
            //telemetry.Labels = new String[] {"label1", "label2", "label3", "label4", "label5"};
            telemetry.Init();
            telemetry.SubscribeToPacket((byte)MessageType.TELEMETRY);
            telemetry.Parent = telemetryTab;

            telemetryTab.Add(telemetry);
            
            /*
            Label label = new Label(manager);
            label.Init();
            label.Color = Color.Red;
            label.Text = "testing";
            label.Top = 0;
            label.Left = 0;
            label.Parent = telemetryTab;
            */
        }

        private void setupGraph(Manager manager, ContentLibrary contentLibrary, TabControl root)
        {

            TabPage graphTab = root.AddPageBeforeEnd();
            graphTab.Text = "graphTestTab";

            graphTab.Init();

            GraphControl graph = new GraphControl(manager, "telemetry", "LRT14.AerialAssist.TELEMETRY", contentLibrary);

            graph.Width = graphTab.ClientWidth;
            graph.Height = graphTab.ClientHeight;

            graph.Init();
            //graph.SubscribeToPacket((byte)MessageType.TELEMETRY);
            graph.Parent = graphTab;

            graphTab.Add(graph);

        }
    }
}
 