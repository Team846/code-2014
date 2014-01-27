using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;

using RhesusNet.NET;

using Dashboard.Library;

namespace LRT14
{
    public class RobotLocationControl : NetworkedControl
    {
        private Color _backgroundColor;
        private Color _robotColor;

        private Color _fieldColor; // TODO change to a Texture2D

        private Vector2 _initialPos;
        private Vector2 _relativePos;

        private float _theta;

        private float _pixelsPerFoot;

        private float _fieldWidthFt;
        private float _fieldHeightFt;

        private float _robotWidthFt;
        private float _robotHeightFt;

        public Color BackgroundColor
        {
            get { return _backgroundColor; }
            set { _backgroundColor = value; }
        }

        public Color RobotColor
        {
            get { return _robotColor; }
            set { _robotColor = value; }
        }

        public Color FieldColor
        {
            get { return _fieldColor; }
            set { _fieldColor = value; }
        }

        public float PixelsPerFoot
        {
            get { return _pixelsPerFoot; }
            set { _pixelsPerFoot = value; }
        }

        public float FeetPerPixel
        {
            get { return 1f / PixelsPerFoot; }
            set { PixelsPerFoot = 1f / value; }
        }

        public float FieldWidthFeet
        {
            get { return _fieldWidthFt; }
            set { _fieldWidthFt = value; }
        }

        public float FieldHeightFeet
        {
            get { return _fieldHeightFt; }
            set { _fieldHeightFt = value; }
        }

        public float RobotWidthFeet
        {
            get { return _robotWidthFt; }
            set { _robotWidthFt = value; }
        }

        public float RobotHeightFeet
        {
            get { return _robotHeightFt; }
            set { _robotHeightFt = value; }
        }

        public Vector2 InitialPosition
        {
            get { return _initialPos; }
            set { _initialPos = value; }
        }

        public Vector2 RelativePosition
        {
            get { return _relativePos; }
            set { _relativePos = value; }
        }

        public float AngleDegrees
        {
            get { return _theta; }
            set { _theta = value; }
        }

        public RobotLocationControl(Manager manager, string id, ContentLibrary content)
            : base(manager, id, content)
        {
            
        }

        private void AddDataPoint(double x, double y, double theta)
        {
            _relativePos.X = (float)x;
            _relativePos.Y = (float)y;
            _theta = (float)theta;
        }

        public override void UpdateControl(GameTime gameTime)
        {
            NetBuffer nb;

            while ((nb = ReadMessage()) != null)
            {
                double x = nb.ReadFloat();
                double y = nb.ReadFloat();
                double theta = nb.ReadFloat();

                AddDataPoint(x, y, theta);
            }

            Invalidate();

            base.UpdateControl(gameTime);
        }

        protected override void DrawControl(Renderer renderer, Rectangle rect, GameTime gameTime)
        {
            // clear BG
            renderer.Draw(Content.DummyTexture, rect, _backgroundColor);

            int controlWidth = Width;
            int controlHeight = Height;

            int fieldWidth = FeetToPixels(FieldWidthFeet);
            int fieldHeight = FeetToPixels(FieldHeightFeet);

            Rectangle fieldRect = new Rectangle(rect.X + rect.Width / 2 - fieldWidth / 2, rect.Y + rect.Height / 2 - fieldHeight / 2, fieldWidth, fieldHeight);

            renderer.Draw(Content.DummyTexture, fieldRect, _fieldColor);

            Vector2 robotFieldPos = InitialPosition + RelativePosition;
            robotFieldPos.X = FeetToPixels(robotFieldPos.X);
            robotFieldPos.Y = FeetToPixels(robotFieldPos.Y);

            int robotWidth = FeetToPixels(RobotWidthFeet);
            int robotHeight = FeetToPixels(RobotHeightFeet);

            Rectangle robotRect = new Rectangle(fieldRect.X + (int)robotFieldPos.X,
                fieldRect.Y + (int)robotFieldPos.Y,
                robotWidth,
                robotHeight);

            renderer.SpriteBatch.Draw(Content.DummyTexture, robotRect, null, _robotColor, _theta * (float)Math.PI / 180f, new Vector2(0.5f, 0.5f), SpriteEffects.None, 0);
            //renderer.Draw(Content.DummyTexture, robotRect, _robotColor, );
        }

        private int FeetToPixels(float feet)
        {
            return (int)(feet * PixelsPerFoot);
        }
    }
}
