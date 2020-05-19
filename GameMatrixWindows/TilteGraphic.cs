using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace GameMatrix
{
    public class TilteGraphic : Tilte
    {
        private static List<TextBlock> CoordOfTilteTextBlocks = new List<TextBlock>();
        static private bool showCoordOfTilte = false;
        static public bool ShowCoordOfTilte
        {
            get
            {
                return showCoordOfTilte;
            }

            set
            {
                showCoordOfTilte = value;
                foreach(TextBlock t in CoordOfTilteTextBlocks)
                {
                    t.Visibility = Visibility.Collapsed;
                }
            }
        }
        static private SolidColorBrush GetBrush(States state)
        {
            switch (state)
            {
                case States.EMPTY: return (SolidColorBrush)(new BrushConverter().ConvertFrom("#024484"));
                case States.LOCK: return (SolidColorBrush)(new BrushConverter().ConvertFrom("#E96658"));
                case States.START: return (SolidColorBrush)(new BrushConverter().ConvertFrom("#5CE484"));
                case States.CHOISE: return (SolidColorBrush)(new BrushConverter().ConvertFrom("#8048F3"));
                case States.VALIDATED: return (SolidColorBrush)(new BrushConverter().ConvertFrom("#FBE423"));



                default: return null;
            }
        }

        public override States State
        {
            get
            {
                return base.State;
            }

            set
            {
                base.State = value;
                button.Background = GetBrush(base.State);
            }
        }

        private Button button = new Button();
        private TextBlock textBlock = new TextBlock();


        public TilteGraphic(int x, int y, Grid mainGrid, Style ButtonStyle) :
            base(x, y)
        {
            Grid.SetColumn(button, x);
            Grid.SetRow(button, y);
            button.Style = ButtonStyle;
            button.Click += Button_Click;
            mainGrid.Children.Add(button);

            Grid.SetColumn(textBlock, x);
            Grid.SetRow(textBlock, y);
            textBlock.Text = String.Format("[{0},{1}]", x, y);
            textBlock.HorizontalAlignment = HorizontalAlignment.Left;
            textBlock.VerticalAlignment = VerticalAlignment.Top;
            textBlock.Foreground = new SolidColorBrush(Colors.White);
            textBlock.Margin = new Thickness(30, 0, 0, 0);
            mainGrid.Children.Add(textBlock);
            CoordOfTilteTextBlocks.Add(textBlock);

            ShowCoordOfTilte = false;

        }

        private void Button_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            OnTilteClicked(new TilteClickedEventArgs(this));
        }

        protected virtual void OnTilteClicked(TilteClickedEventArgs e)
        {
            TilteClicked?.Invoke(this, e);
        }

        public event EventHandler<TilteClickedEventArgs> TilteClicked;


        public class TilteClickedEventArgs : EventArgs
        {
            public TilteClickedEventArgs(Tilte t)
            {
                Tilte = t;
            }
            public Tilte Tilte { get; private set; }
        }

    }
}
