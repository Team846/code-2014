using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dashboard.Library.Persistence;
using Microsoft.Xna.Framework;
using System.IO;
using RhesusNet.NET;
using System.Diagnostics.Contracts;
using TomShane.Neoforce.Controls;
using Dashboard.Library;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Graphics;

namespace LRT14
{
    public class RobotLocationAnalysisControl : AnalysisControl
    {
        public class LocationOptionsDialog : Dialog
        {
            private Label _initPosXLabel;
            private TextBox _initPosXInput;

            private Label _initPosYLabel;
            private TextBox _initPosYInput;

            private Button _saveButton;
            private Button _cancelButton;

            public LocationOptionsDialog(Manager manager)
                : base(manager)
            {
                string msg = "Do you really want to exit " + Manager.Game.Window.Title + "?";
                //ClientWidth = (int)Manager.Skin.Controls["Label"].Layers[0].Text.Font.Resource.MeasureString(msg).X + 48 + 16 + 16 + 16;
                ClientWidth = 300;
                ClientHeight = 200;
                TopPanel.Visible = false;
                IconVisible = false;
                Resizable = false;
                Text = "Location Options";
                Center();

                _initPosXLabel = new Label(manager);
                _initPosXLabel.Text = "Initial X (ft)";
                _initPosXLabel.Width = (int)Manager.Skin.Controls["Label"].Layers[0].Text.Font.Resource.MeasureString("Initial X (ft)").X;
                _initPosXLabel.Top = 4;
                _initPosXLabel.Left = 4;
                _initPosXLabel.Init();

                _initPosXInput = new TextBox(manager);
                _initPosXInput.Mode = TextBoxMode.Normal;
                _initPosXInput.Top = 4;
                _initPosXInput.Left = 4 + 4 + _initPosXLabel.Width;
                _initPosXInput.Init();

                _initPosYLabel = new Label(manager);
                _initPosYLabel.Text = "Initial Y (ft)";
                _initPosYLabel.Width = (int)Manager.Skin.Controls["Label"].Layers[0].Text.Font.Resource.MeasureString("Initial Y (ft)").X;
                _initPosYLabel.Top = 4 + (int)Manager.Skin.Controls["Label"].Layers[0].Text.Font.Resource.MeasureString("Initial X (ft)").Y + 2 + 4;
                _initPosYLabel.Left = 4;
                _initPosYLabel.Init();

                _initPosYInput = new TextBox(manager);
                _initPosYInput.Mode = TextBoxMode.Normal;
                _initPosYInput.Top = _initPosYLabel.Top;
                _initPosYInput.Left = 4 + 4 + _initPosYLabel.Width;
                _initPosYInput.Init();

                _saveButton = new Button(Manager);
                _saveButton.Init();
                _saveButton.Left = (BottomPanel.ClientWidth / 2) - _saveButton.Width - 8;
                _saveButton.Top = 8;
                _saveButton.Text = "Save";
                _saveButton.ModalResult = ModalResult.Ok;

                _cancelButton = new Button(Manager);
                _cancelButton.Init();
                _cancelButton.Left = (BottomPanel.ClientWidth / 2) + 8;
                _cancelButton.Top = 8;
                _cancelButton.Text = "Cancel";
                _cancelButton.ModalResult = ModalResult.Cancel;   

                Add(_initPosXLabel);
                Add(_initPosXInput);
                Add(_initPosYLabel);
                Add(_initPosYInput);

                BottomPanel.Add(_saveButton);
                BottomPanel.Add(_cancelButton);
            }
        }

        private Color _backgroundColor;
        private Color _robotColor;

        private Color _fieldColor; // TODO change to a Texture2D

        private Vector2 _initialPos;
        private Vector2 _relativePos;

        private RobotLocationData _data;

        private float _theta;

        private float _pixelsPerFoot;

        private float _fieldWidthFt;
        private float _fieldHeightFt;

        private float _robotWidthFt;
        private float _robotHeightFt;

        public IPersistable Data
        {
            get { return _data; }

        }

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
            get { return new Vector2(_data.GetValue().X, _data.GetValue().Y); }
        }

        public float AngleDegrees
        {
            get { return _theta; }
            set { _theta = value; }
        }

        public RobotLocationAnalysisControl(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
            MouseOver += new MouseEventHandler(RobotLocationControl_MouseOver);
            MouseOut += new MouseEventHandler(RobotLocationControl_MouseOut);

            _optionsOpen = false;

            _isMousedOver = false;

            _zoom = 1.0f;

            _data = new RobotLocationData();
            PersistenceManager.Persistence.Set(PersistenceKey, _data);

            Manager.PreDraws.Add(this);
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

        private int _lastScrollWheelValue;
        private const int SCROLL_UNIT = 100;
        private bool _isScrollWheelInitialized;
        private float _zoom;

        private bool _optionsOpen;

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

                _lastScrollWheelValue = currentScroll;
            }

            KeyboardState kstate = Keyboard.GetState();

            if (!_optionsOpen && kstate.IsKeyDown(Keys.O))
            {
                LocationOptionsDialog lod = new LocationOptionsDialog(Manager);
                lod.Init();
                lod.Closed += new WindowClosedEventHandler(lod_Closed);
                lod.ShowModal();
                Manager.Add(lod);
                _optionsOpen = true;
            }

            Invalidate();

            base.UpdateControl(gameTime);
        }

        void lod_Closed(object sender, WindowClosedEventArgs e)
        {
            _optionsOpen = false;
        }

        RenderTarget2D _canvas;

        public override void PreDraw(Renderer renderer)
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

            renderer.SpriteBatch.GraphicsDevice.SetRenderTarget(_canvas);
            renderer.SpriteBatch.GraphicsDevice.Clear(_backgroundColor);
            renderer.SpriteBatch.Begin();

            int controlWidth = Width;
            int controlHeight = Height;

            int fieldWidth = FeetToPixels(FieldWidthFeet);
            int fieldHeight = FeetToPixels(FieldHeightFeet);

            Rectangle fieldRect = new Rectangle(Width / 2 - fieldWidth / 2, Height / 2 - fieldHeight / 2, fieldWidth, fieldHeight);

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

            renderer.SpriteBatch.End();
            renderer.SpriteBatch.GraphicsDevice.SetRenderTarget(null);
        }

        protected override void DrawControl(Renderer renderer, Rectangle rect, GameTime gameTime)
        {
            // clear BG
            renderer.Draw(Content.DummyTexture, rect, _backgroundColor);

            renderer.SpriteBatch.Draw(_canvas, new Vector2(rect.Center.X, rect.Center.Y), null, Color.White, 0, new Vector2(_canvas.Width / 2f, _canvas.Height / 2f), _zoom, SpriteEffects.None, 0);
        }

        private int FeetToPixels(float feet)
        {
            return (int)(feet * PixelsPerFoot);
        }
    }
}
