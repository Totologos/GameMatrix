using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace GameMatrix
{
    class AutoPlay
    {

        private int numberOfGames = 0;
        private int numberOfWinabbleGames = 0;
        private int numberOfLoose = 0;

        private Dictionary<int, List<String>> listOfWinParty = new Dictionary<int, List<String>>();
        
        public String Result { get; private set; }

        public AutoPlay(List<Tilte> tiltes)
        {
            GamePlay gamePlay = new GamePlay(tiltes);
 

            var StartTiltes = tiltes.Where(_ => _.State != Tilte.States.LOCK).ToList();
            List<GamePlay> gamePlays = new List<GamePlay>();

            foreach (Tilte st in StartTiltes)
            {
                gamePlays.Add(new GamePlay(gamePlay));
            }

            for(int i = 0; i < StartTiltes.Count; i++)
            { 
                Play(gamePlays[i], StartTiltes[i], new List<String>());
            }


            Result = String.Format("Number of games: {0}", numberOfGames) + Environment.NewLine;
            Result += String.Format("Number of winable games: {0}", numberOfWinabbleGames) + Environment.NewLine;
            Result += String.Format("Ratio: {0:0.00}%", ((double)numberOfWinabbleGames / (double)numberOfGames) * 100.0) + Environment.NewLine;

            foreach (KeyValuePair<int, List<String>> entry in listOfWinParty)
            {
                Result += String.Format("------ {0} ------", entry.Key) + Environment.NewLine;
                foreach (String s in entry.Value)
                {
                    Result += s + Environment.NewLine;
                }
            }

        }

        private void Play(GamePlay gamePlay, Tilte eventTilte, List<String> previousStrEvent)
        {
            numberOfGames++;

            previousStrEvent.Add(String.Format("[{0}, {1}]", eventTilte.X, eventTilte.Y));
            gamePlay.TilteEvent(eventTilte.X, eventTilte.Y);

            while (true)
            {
                var chooseTiltes = gamePlay.Tiltes.Where(_ => _.State == Tilte.States.CHOISE).ToList();

                if(chooseTiltes.Count == 0)
                {
                    numberOfLoose++;
                    return;
                }
                if (chooseTiltes.Count == 1)
                {
                    previousStrEvent.Add(String.Format("[{0}, {1}]", chooseTiltes[0].X, chooseTiltes[0].Y)); 
                    gamePlay.TilteEvent(chooseTiltes[0].X, chooseTiltes[0].Y);

                    if (gamePlay.GameSequence == GamePlay.GameSequences.RESTART)
                    {
                        if (gamePlay.Win)
                        {
                            listOfWinParty.Add(numberOfWinabbleGames, previousStrEvent);
                            numberOfWinabbleGames++;
                        }
                        else
                        {
                            numberOfLoose++;
                        }
                        return;
                    }

                }
                else
                {
                    

                    string choiseString = "";
                    foreach (Tilte tc in chooseTiltes)
                    {
                        choiseString += String.Format("[{0}, {1}] ", tc.X, tc.Y);
                    }

                    previousStrEvent.Add(String.Format("choose of {0}: ", chooseTiltes.Count) + choiseString);

                    List<GamePlay> gp = new List<GamePlay>();
                    List<List<String>> pe = new List<List<String>>();

                    foreach (Tilte tc in chooseTiltes)
                    {
                        gp.Add(new GamePlay(gamePlay));
                        pe.Add(new List<String>(previousStrEvent));
                    }

                    for (int i = 0; i < chooseTiltes.Count; i++)
                    {
                        Play(gp[i], chooseTiltes[i], pe[i]);
                    }



                    return;                  
                }
            }
            

        }
    }
}
