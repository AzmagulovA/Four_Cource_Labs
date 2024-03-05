using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public class Rook:Piece
    {
        public override PieceType Type => PieceType.Rook;//override - переназначенный тип базового класса
        public override Player Color { get; }
        private static readonly Direction[] dirs = new Direction[]
        {
            Direction.North,
            Direction.West,
            Direction.South,
            Direction.East
        };
        public Rook(Player color)
        {
            Color = color;

        }
       public override Piece Copy()
        {
            Rook copy = new Rook(Color);
            copy.HasMoved = HasMoved;
            return copy;
        }
        public override IEnumerable<Move> GetMoves(Position from, Board board)
        {
            return MovePositionInDirs(from, board, dirs).Select(to => new NormalMove(from, to));//получаем множество доступных позиций, для каждого из которых берется нормальный ход и помещается туда
        }

    }
}
