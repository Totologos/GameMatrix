using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameMatrix
{
    
    public class GamePlay
    {
        public enum GameSequences
        {
            SELECT_START_POINT,
            MOOVED,
            END,
            RESTART,
        }

        public GameSequences GameSequence { get; protected set; }

        public List<Tilte> Tiltes { get; protected set; }

        public GamePlay(List<Tilte> tiltes)
        {
            GameSequence = GameSequences.SELECT_START_POINT;
            Win = false;
            Tiltes = tiltes;
        }

        public GamePlay(GamePlay prevGamePlay)
        {
            GameSequence = prevGamePlay.GameSequence;
            tiltePrev = prevGamePlay.tiltePrev;

            //Tiltes = new List<Tilte>(prevGamePlay.Tiltes);
            Tiltes = new List<Tilte>();
            foreach (Tilte pt in prevGamePlay.Tiltes)
            {
                Tiltes.Add(new Tilte(pt));

            }
            Win = prevGamePlay.Win;
        }

        public bool Win { get; protected set; }
               
        private Tilte tiltePrev = null;


        public void TilteEvent(int x, int y)
        {
            TilteEvent( Tiltes.FirstOrDefault(_ => _.X == x && _.Y == y));
        }

        protected void TilteEvent(Tilte t)
        {

            switch (GameSequence)
            {
                case GameSequences.SELECT_START_POINT:
                    if (t.State == Tilte.States.EMPTY)
                    {
                        t.State = Tilte.States.START;

                        if (Update(t))
                        {
                            // player must be do a choice !
                            GameSequence = GameSequences.MOOVED;
                        }
                        else
                        {
                            GameSequence = GameSequences.END;
                            TilteEvent(t);
                        }
                    }
                    break;
                case GameSequences.MOOVED:
                    if( t.State == Tilte.States.START)
                    {
                        ClearTiltles();
                        GameSequence = GameSequences.SELECT_START_POINT;
                        tiltePrev = null;
                    }
                    else if (t.State == Tilte.States.CHOISE)
                    {
                        t.State = Tilte.States.VALIDATED;
                        RemoveChoiseTiltles();
                        if (Update(t))
                        {
                            // player must be do a choice !
                            GameSequence = GameSequences.MOOVED;
                            
                        }
                        else
                        {
                            GameSequence = GameSequences.END;
                            TilteEvent(t);
                        }
                    }
                    break;
                case GameSequences.END:
                    if(NoEmptyTilte())
                    {
                        Win = true;
                        AllStartTiltes();
                        
                    }
                    GameSequence = GameSequences.RESTART;
                    break;

                case GameSequences.RESTART:
                    ResetGame();
                    break;

            }
        }

        public void ResetGame()
        {
            Win = false;
            ClearTiltles();
            GameSequence = GameSequences.SELECT_START_POINT;
            tiltePrev = null;
        }

        private bool Update(Tilte t)
        {
            // Auto move in the selected direction...
            if (tiltePrev != null)
            {
                if(t.Y == tiltePrev.Y && t.X - 1 == tiltePrev.X)
                {
                    if (UpdateTilteState(Tilte.States.VALIDATED, t.X + 1, t.Y))
                    {
                        tiltePrev = t;
                        return Update(GetTilte(t.X + 1, t.Y));
                    }
                        
                }
                else if (t.Y == tiltePrev.Y && t.X + 1 == tiltePrev.X)
                {
                    if (UpdateTilteState(Tilte.States.VALIDATED, t.X - 1, t.Y))
                    {
                        tiltePrev = t;
                        return Update(GetTilte(t.X - 1, t.Y));
                    }
                        
                }
                else if (t.Y - 1 == tiltePrev.Y && t.X == tiltePrev.X)
                {
                    if (UpdateTilteState(Tilte.States.VALIDATED, t.X, t.Y + 1))
                    {
                        tiltePrev = t;
                        return Update(GetTilte(t.X, t.Y + 1));
                    }
                        
                }
                else if (t.Y + 1 == tiltePrev.Y && t.X == tiltePrev.X)
                {
                    if (UpdateTilteState(Tilte.States.VALIDATED, t.X, t.Y - 1))
                    {
                        tiltePrev = t;
                        return Update(GetTilte(t.X, t.Y - 1));
                    }
                        
                }
                
            }
            tiltePrev = t;

            bool canMoove = false;
            canMoove |= CanMoove(t.X, t.Y - 1);
            canMoove |= CanMoove(t.X, t.Y + 1);
            canMoove |= CanMoove(t.X - 1, t.Y);
            canMoove |= CanMoove(t.X + 1, t.Y);

            if ( canMoove )
            {

                UpdateTilteState(Tilte.States.CHOISE, t.X, t.Y - 1);
                UpdateTilteState(Tilte.States.CHOISE, t.X, t.Y + 1);
                UpdateTilteState(Tilte.States.CHOISE, t.X - 1, t.Y);
                UpdateTilteState(Tilte.States.CHOISE, t.X + 1, t.Y);
                
                return true;                
            }
            else 
            {
                return false;
                // end !
            }
        }

        private bool UpdateTilteState(Tilte.States newState, int x, int y)
        {
            var t = Tiltes.FirstOrDefault(_ => (_.X == x && _.Y == y));
            if (t != null && t.State == Tilte.States.EMPTY)
            {
                t.State = newState;
                return true;
            }
            return false;
        }

        private Tilte GetTilte(int x, int y)
        {
            return Tiltes.FirstOrDefault(_ => (_.X == x && _.Y == y));
        }

        private bool CanMoove(int x, int y)
        {
            var t = Tiltes.FirstOrDefault(_ => (_.X == x && _.Y == y));
            if (t != null && t.State == Tilte.States.EMPTY)
            {
                return true;
            }
            return false;
        }

        private void RemoveChoiseTiltles()
        {
            foreach(var  t in Tiltes.Where(_ => _.State == Tilte.States.CHOISE))
            {
                t.State = Tilte.States.EMPTY;
            }
        }

        private void ClearTiltles()
        {
            foreach (var t in Tiltes.Where(_ => _.State != Tilte.States.LOCK))
            {
                t.State = Tilte.States.EMPTY;
            }
        }

        private bool NoEmptyTilte()
        {
            return Tiltes.Count(_ => _.State == Tilte.States.EMPTY) == 0;
        }

        private void AllStartTiltes()
        {
            foreach (var t in Tiltes.Where(_ => _.State != Tilte.States.LOCK))
            {
                t.State = Tilte.States.START;
            }
        }
    }
}
