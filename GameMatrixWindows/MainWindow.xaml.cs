using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace GameMatrix
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //private List<Tilte> TiltesGraphic = new List<Tilte>();
        private List<Tilte> Tiltes = new List<Tilte>();

        private GamePlayGraphic gp;

        private int row = 7;
        private int column = 7;

        public MainWindow()
        {
            InitializeComponent();
            Style s = Resources["ButtonStyleNoHighlighting"] as Style;

            for(int i=0; i < row; i++)
            {
                mainGrid.RowDefinitions.Add(new RowDefinition());
            }

            for (int i = 0; i < column; i++)
            {
                mainGrid.ColumnDefinitions.Add(new ColumnDefinition());
            }


            for (int i = 0; i < column; i++)
            {
                for (int j = 0; j < row; j++)
                {
                    Tiltes.Add(new TilteGraphic(i, j, mainGrid, s));
                    //Tiltes.Add(new Tilte(i, j));
                }
            }

            // puzzle 2 
            //Tiltes.FirstOrDefault(t => t.X == 3 && t.Y == 1).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 4 && t.Y == 4).State = Tilte.States.LOCK;

            // puzzle 3 
            //Tiltes.FirstOrDefault(t => t.X == 4 && t.Y == 2).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 2 && t.Y == 2).State = Tilte.States.LOCK;

            // puzzle 4 
            //Tiltes.FirstOrDefault(t => t.X == 1 && t.Y == 1).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 3 && t.Y == 1).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 1 && t.Y == 3).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 2 && t.Y == 3).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 4 && t.Y == 3).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 4 && t.Y == 4).State = Tilte.States.LOCK;

            // puzzle 5
            //Tiltes.FirstOrDefault(t => t.X == 2 && t.Y == 0).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 3 && t.Y == 0).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 4 && t.Y == 0).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 0 && t.Y == 4).State = Tilte.States.LOCK;
            //Tiltes.FirstOrDefault(t => t.X == 1 && t.Y == 4).State = Tilte.States.LOCK;

            SequenceButton.Click += SequenceButton_Click;
            

            AutoPlay autoPlay = new AutoPlay(Tiltes);
            LogTextBlock.Text = autoPlay.Result;
            gp = new GamePlayGraphic(Tiltes);
            gp.TilteClickedInEditMode += Gp_TilteClickedInEditMode;

            SequenceButton_Click(null, null);
        }

        private void Gp_TilteClickedInEditMode(object sender, GamePlayGraphic.TilteClickedInEditModeEventArgs e)
        {
            AutoPlay autoPlay = new AutoPlay(Tiltes);
            LogTextBlock.Text = autoPlay.Result;
        }


        private void SequenceButton_Click(object sender, RoutedEventArgs e)
        {
            if(gp.Mode == GamePlayGraphic.Modes.EDIT)
            {
                gp.Mode = GamePlayGraphic.Modes.PLAY;
                SequenceButton.Content = "PLAY MODE\r\n(click to edit)";
                gp.ResetGame();
            }
            else
            {
                SequenceButton.Content = "EDIT MODE\r\n(click to play)";
                gp.Mode = GamePlayGraphic.Modes.EDIT;
                foreach(Tilte t in Tiltes)
                {
                    if(t.State != Tilte.States.LOCK)
                    {
                        t.State = Tilte.States.EMPTY;
                    }
                }
            }
        }
    }
}
