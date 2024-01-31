using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public abstract class Piece//аbstract - неполный класс, предназначенный для обозначения базового класаа для других классов
    {
        public abstract PieceType Type { get; }
        public abstract Player Color { get; }//цвет
        public bool HasMoved { get; set; } = false;// 

        public abstract Piece Copy();

        public abstract IEnumerable<Move> GetMoves(Position from, Board board);

        protected IEnumerable<Position> MovePositionInDir(Position from,Board board,Direction dir)//проверка достижимости клетки фигурой
        {
            for(Position pos = from + dir; Board.IsInside(pos); pos += dir)
            {
                if (board.IsEmpty(pos))//если клетка пуста в каком-то допустимом напрвлении, то она достижима
                {
                    yield return pos;
                    continue;
                }
                Piece piece = board[pos];//иначе на ней кто-то стоит и мы проверяем ее на цвет
                if (piece.Color != Color)
                {
                    yield return pos;//и если цвет чужой, то мы можем поразить эту клетку
                }
                yield break;
            }
        }
        protected IEnumerable<Position> MovePositionInDirs(Position from,Board board, Direction[] directions)//все возможные ходы в заданных направлениях
        {
            return directions.SelectMany(dir => MovePositionInDir(from,board,dir));
        }

    }
}
