using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using TomShane.Neoforce.Controls;

using RhesusNet.NET;

namespace Dashboard.Library
{
    public class Graph : DashboardControl
    {
        private Color _backgroundColor;
        private Color _lineColor;
        private Color _leadingDotColor;

        private List<Vector2> _dataPoints;

        // TODO change me to properties
        public float PixelsPerTimeTick_x, PixelsPerTimeTick_y, MaxValue;

        int graph_baseline;

        private string _id;

        public Color BackgroundColor
        {
            get { return _backgroundColor; }
            set { _backgroundColor = value; }
        }

        public Color LineColor
        {
            get { return _backgroundColor; }
            set { _backgroundColor = value; }
        }

        public Color LeadingDotColor
        {
            get { return _backgroundColor; }
            set { _backgroundColor = value; }
        }

        public float RenderOffset
        {
            get
            {
                return Math.Max(0, _dataPoints[_dataPoints.Count - 1].X * PixelsPerTimeTick_x - 0.9f * Width);
            }
        }

        public Graph(Manager manager, string id, string persistenceKey, ContentLibrary content)
            : base(manager, id, persistenceKey, content)
        {
            _backgroundColor = Color.White;
            _lineColor = Color.Blue;
            _leadingDotColor = Color.Red;

            _dataPoints = new List<Vector2>();

            PixelsPerTimeTick_x = this.Height;
            PixelsPerTimeTick_y = this.Width;
            MaxValue = 0;
            _id = id;
        }

        public override void UpdateControl(GameTime gameTime)
        {
            graph_baseline = Height - 2;

            Invalidate();

            base.UpdateControl(gameTime);
        }

        public override void SubscribeToPacket(byte header)
        {
            throw new InvalidOperationException();
        }

        public void AddDataPoint(Vector2 data)
        {
            UpdateMaxValue(data);
            _dataPoints.Add(data);
 
        }

        protected void UpdateMaxValue(Vector2 data)
        {

            if (data.Y > MaxValue)
            {
                MaxValue = data.Y;

                PixelsPerTimeTick_y = MaxValue;
            }


        }

        protected Vector2 ScaledVersion(Vector2 data)
        {


            return new Vector2(data.X, data.Y / MaxValue);
        }

        private void DrawAxes(Renderer renderer, Rectangle rect, GameTime gameTime)
        {

        }

        private void DrawPoints(Renderer renderer, Rectangle rect, GameTime gameTime)
        {
            for (int i = _dataPoints.Count - 1; i > 0; --i)
            {
                Vector2 v20 = _dataPoints[i - 1];
                v20.X *= PixelsPerTimeTick_x;
                v20.Y *= PixelsPerTimeTick_y;
                v20.X -= RenderOffset;
                Vector2 v21 = _dataPoints[i];
                v21.X *= PixelsPerTimeTick_x;
                v21.Y *= PixelsPerTimeTick_y;
                v21.X -= RenderOffset;

                if (v21.X < 0 || v21.X >= Width)
                    break;

                float dy = v21.Y - v20.Y;
                float dx = v21.X - v20.X;

                //Invert the graph system to compensate for screen space coordinates
                v20.Y = (-v20.Y) + graph_baseline;
                v21.Y = (-v21.Y) + graph_baseline;
                
                v20.X += rect.X;
                v20.Y += rect.Y;

                v21.X += rect.X;
                v21.Y += rect.Y;

                renderer.SpriteBatch.Draw(Content.DummyTexture, v20, null, _lineColor, (float)-Math.Atan2(dy, dx), new Vector2(0, 0), new Vector2(Vector2.Distance(v20, v21), 1f), SpriteEffects.None, 0);

                if(i == _dataPoints.Count - 1)
                    renderer.SpriteBatch.Draw(Content.DummyTexture, new Rectangle((int)v21.X, (int)v21.Y, 5, 5), null, _leadingDotColor, (float)-Math.Atan2(dy, dx), new Vector2(0.5f, 0.5f), SpriteEffects.None, 0);
            }
        }

        protected override void DrawControl(Renderer renderer, Rectangle rect, GameTime gameTime)
        {
            // clear BG
            renderer.Draw(Content.DummyTexture, rect, _backgroundColor);

            DrawPoints(renderer, rect, gameTime);
            DrawPoints(renderer, rect, gameTime);
        }


    }
}
