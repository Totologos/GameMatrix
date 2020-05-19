using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameMatrix
{
    public class GamePlayGraphic : GamePlay
    {
        public GamePlayGraphic(List<Tilte> tiltes) : base(tiltes)
        {
            foreach (Tilte t in Tiltes)
            {
                (t as TilteGraphic).TilteClicked += T_TilteClicked;
            }
        }

        private void T_TilteClicked(object sender, TilteGraphic.TilteClickedEventArgs e)
        {
            if (Mode == Modes.PLAY)
            {
                TilteEvent(e.Tilte);
            }
            else if (e.Tilte.State == Tilte.States.LOCK)
            {
                e.Tilte.State = Tilte.States.EMPTY;
                OnTilteClickedInEditMode(new TilteClickedInEditModeEventArgs(Tiltes));
            }
            else
            {
                e.Tilte.State = Tilte.States.LOCK;
                OnTilteClickedInEditMode(new TilteClickedInEditModeEventArgs(Tiltes));
            }

        }

        public Modes Mode { get; set; }

        public enum Modes
        {
            EDIT,
            PLAY
        };

        protected virtual void OnTilteClickedInEditMode(TilteClickedInEditModeEventArgs e)
        {
            TilteClickedInEditMode?.Invoke(this, e);
        }

        public event EventHandler<TilteClickedInEditModeEventArgs> TilteClickedInEditMode;


        public class TilteClickedInEditModeEventArgs : EventArgs
        {
            public TilteClickedInEditModeEventArgs()
            {

            }
            public TilteClickedInEditModeEventArgs(List<Tilte> t)
            {
                Tiltes = t;
            }
            public List<Tilte> Tiltes { get; private set; }
        }
    }
}
