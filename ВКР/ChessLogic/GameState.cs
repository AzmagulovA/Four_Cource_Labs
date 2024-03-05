using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public class GameState
    {
        public Board Board { get; }
        public Player CurrentPlayer { get; private set; }

        public Result Result { get; private set; } = null;
        public Position LastFrom = null;
        public Position LastTo = null;
        public GameState(Player player,Board board)
        {
            CurrentPlayer = player;
            Board = board;
        }
        public IEnumerable<Move> LegalMovesForPiece(Position pos)
        {
            if(Board.IsEmpty(pos)|| Board[pos].Color != CurrentPlayer)//если клетка доски пуста или там не наша фигура, то нет доступных ходов
            {
                return Enumerable.Empty<Move>();
            }
            Piece piece = Board[pos];//иначе берем эту фигуру и смотрим доступные ходы
            IEnumerable<Move> moveCandidates= piece.GetMoves(pos,Board);
            return moveCandidates.Where(move => move.IsLegal(Board));
        }
        public void MakeMove(Move move)
        {
            Board.SetPawnPosition(CurrentPlayer, null);
            move.Execute(Board);
            LastFrom = move.FromPos;
            LastTo = move.ToPos;
            CurrentPlayer = CurrentPlayer.Opponent();
            CheckForGameOver();
        }

        private IEnumerable<Move> AllLegalMovesFor(Player player)//все легальные возможные ходы игрока
        {
            IEnumerable<Move> moveCandidates = Board.PiecePositionsFor(player).SelectMany(pos =>
            {
                Piece piece = Board[pos];
                return piece.GetMoves(pos, Board);
            });

            return moveCandidates.Where(move => move.IsLegal(Board));

        }
        private void CheckForGameOver()//проверка на окончание партии после каждого хода
        {
            if (!AllLegalMovesFor(CurrentPlayer).Any())//если у игрока нет ходов
            {
                if (Board.IsInCheck(CurrentPlayer))//и король под шахом
                {
                    Result = Result.Win(CurrentPlayer.Opponent());//победа противника

                }
                else
                {
                    Result = Result.Draw(EndReason.Stalemate);//пат
                }
            }

        }
        public bool IsGameOver()
        {
            return Result != null;
        }

    }
}
