﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;
using STDConsole = System.Console;

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
            MouseOver += new MouseEventHandler(RobotLocationControl_MouseOver);
            MouseOut += new MouseEventHandler(RobotLocationControl_MouseOut);

            _isMousedOver = false;

            _zoom = 1.0f;
        }

        bool _isMousedOver;

        void RobotLocationControl_MouseOut(object sender, MouseEventArgs e)
        {
            _isMousedOver = false;
        }

        void RobotLocationControl_MouseOver(object sender, MouseEventArgs e)
        {
            _isMousedOver = true;
        }

        private void AddDataPoint(double x, double y, double theta)
        {
            _relativePos.X = (float)x;
            _relativePos.Y = (float)y;
            _theta = (float)theta;
        }

        private int _lastScrollWheelValue;
        private const int SCROLL_UNIT = 100;
        private bool _isScrollWheelInitialized;
        private float _zoom;

        public override void UpdateControl(GameTime gameTime)
        {
            NetBuffer nb;

            MouseState mstate = Mouse.GetState();

            if (base.Visible && _isMousedOver)
            {
                if (!_isScrollWheelInitialized)
                {
                    _lastScrollWheelValue = mstate.ScrollWheelValue;
                    _isScrollWheelInitialized = true;
                }

                int currentScroll = mstate.ScrollWheelValue;
                if (currentScroll > _lastScrollWheelValue + SCROLL_UNIT)
                {
                    _zoom += 0.25f;
                }
                else if (currentScroll < _lastScrollWheelValue - SCROLL_UNIT)
                {
                    _zoom -= 0.25f;
                }

                _zoom = MathHelper.Clamp(_zoom, 0.25f, 2f);

                STDConsole.WriteLine(_zoom);

                _lastScrollWheelValue = currentScroll;
            }

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

        RenderTarget2D _canvas;

        protected override void DrawControl(Renderer renderer, Rectangle rect, GameTime gameTime)
        {
            if (_canvas == null)
            {
                _canvas = new RenderTarget2D(renderer.SpriteBatch.GraphicsDevice,
                renderer.SpriteBatch.GraphicsDevice.PresentationParameters.BackBufferWidth,
                renderer.SpriteBatch.GraphicsDevice.PresentationParameters.BackBufferHeight,
                false,
                renderer.SpriteBatch.GraphicsDevice.PresentationParameters.BackBufferFormat,
                DepthFormat.Depth24);
            }

            // clear BG
            renderer.Draw(Content.DummyTexture, rect, _backgroundColor);

            renderer.SpriteBatch.GraphicsDevice.SetRenderTarget(_canvas);

            int controlWidth = Width;
            int controlHeight = Height;

            int fieldWidth = FeetToPixels(FieldWidthFeet);
            int fieldHeight = FeetToPixels(FieldHeightFeet);

            Rectangle fieldRect = new Rectangle(rect.X + rect.Width / 2 - fieldWidth / 2, rect.Y + rect.Height / 2 - fieldHeight / 2, fieldWidth, fieldHeight);

            renderer.SpriteBatch.Draw(Content.DummyTexture, fieldRect, _fieldColor);

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

            renderer.SpriteBatch.GraphicsDevice.SetRenderTarget(null);

            renderer.SpriteBatch.Draw(_canvas, Vector2.Zero, Color.White);
        }

        private int FeetToPixels(float feet)
        {
            return (int)(feet * PixelsPerFoot);
        }
    }
}
