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
            return piece.GetMoves(pos,Board);
        }
        public void MakeMove(Move move)
        {
            move.Execute(Board);
            CurrentPlayer = CurrentPlayer.Opponent();
        }
    }
}
