using System;

namespace GameMatrix
{
    public class Tilte
    {
        public enum States
        {
            EMPTY,
            LOCK,
            START,
            CHOISE,
            VALIDATED
        }

        public virtual States State { get; set; }
        public int X { get; private set; }
        public int Y { get; private set; }

        public Tilte(int x, int y)
        {
            X = x;
            Y = y;
            State = States.EMPTY;
        }

        public Tilte(Tilte previousTilte)
        {
            X = previousTilte.X;
            Y = previousTilte.Y;
            State = previousTilte.State;
        }
    }
}
