using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public class King:Piece
    {
        public override PieceType Type => PieceType.King;//override - переназначенный тип базового класса
        public override Player Color { get; }
        private static readonly Direction[] dirs = new Direction[]
        {
            Direction.North,
            Direction.West,
            Direction.South,
            Direction.East,
            Direction.NorthEast,
            Direction.NorthWest,
            Direction.SouthWest,
            Direction.SouthEast
        };
        public King(Player color)
        {
            Color = color;

        }
        public override Piece Copy()
        {
            King copy = new King(Color);
            copy.HasMoved = HasMoved;
            return copy;
        }
        private IEnumerable<Position> MovePositions(Position from,Board board)
        {
            foreach (Direction dir in dirs)
            {
                Position to = from + dir;
                if (!Board.IsInside(to))//если позиция выходит за пределы доски, то сразу к следующему
                {
                    continue;
                }
                if (board.IsEmpty(to) || board[to].Color!=Color)
                {
                    yield return to;
                }
            }
        }
        public override IEnumerable<Move> GetMoves(Position from, Board board)
        {
            foreach (Position to in MovePositions(from,board))
            {
                yield return new NormalMove(from, to);
            }
        }
    }
}
