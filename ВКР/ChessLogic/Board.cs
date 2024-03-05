using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public class Board
    {
        private readonly Piece[,] pieces = new Piece[8, 8];
        private readonly Dictionary<Player, Position> pawnSkipPositions = new Dictionary<Player, Position>()
        {
            { Player.White, null},
            { Player.Black,null}

        };
        public Piece this[int row, int col]
        {
            get { return pieces[row, col]; }
            set { pieces[row, col] = value; }
        }
        public Piece this[Position pos]
        {
            get { return this[pos.Row, pos.Column]; }
            set { this[pos.Row, pos.Column] = value; }
        }

        public Position GetPawnSkipPosition(Player player)
        {
            return pawnSkipPositions[player];

        }
        public void SetPawnPosition(Player player,Position pos)
        {
            pawnSkipPositions[player] = pos;
        }

        public static Board Initial()
        {
            Board board = new Board();
            board.AddStartPieces();
            return board;
        }
        private void AddStartPieces()
        {
            this[0, 0] = new Rook(Player.Black);
            this[0, 1] = new Knight(Player.Black);
            this[0, 2] = new Bishop(Player.Black);
            this[0, 3] = new Queen(Player.Black);
            this[0, 4] = new King(Player.Black);
            this[0, 5] = new Bishop(Player.Black);
            this[0, 6] = new Knight(Player.Black);
            this[0, 7] = new Rook(Player.Black);

            this[1, 0] = new Pawn(Player.Black);
            this[1, 1] = new Pawn(Player.Black);
            this[1, 2] = new Pawn(Player.Black);
            this[1, 3] = new Pawn(Player.Black);
            this[1, 4] = new Pawn(Player.Black);
            this[1, 5] = new Pawn(Player.Black);
            this[1, 6] = new Pawn(Player.Black);
            this[1, 7] = new Pawn(Player.Black);


            this[7, 0] = new Rook(Player.White);
            this[7, 1] = new Knight(Player.White);
            this[7, 2] = new Bishop(Player.White);
            this[7, 3] = new Queen(Player.White);
            this[7, 4] = new King(Player.White);
            this[7, 5] = new Bishop(Player.White);
            this[7, 6] = new Knight(Player.White);
            this[7, 7] = new Rook(Player.White);

            this[6, 0] = new Pawn(Player.White);
            this[6, 1] = new Pawn(Player.White);
            this[6, 2] = new Pawn(Player.White);
            this[6, 3] = new Pawn(Player.White);
            this[6, 4] = new Pawn(Player.White);
            this[6, 5] = new Pawn(Player.White);
            this[6, 6] = new Pawn(Player.White);
            this[6, 7] = new Pawn(Player.White);
        }
        public static bool IsInside(Position pos)
        {
            return pos.Row >=0 && pos.Row <= 7 && pos.Column>=0 && pos.Column <= 7;
        }
        public bool IsEmpty(Position pos)//проверка на пустоту клетки
        {
            return this[pos] == null;
        }

        public IEnumerable<Position> PiecePositions()//возврат всех позиций в которых фигуры
        {
            for(int r = 0; r < 8; r++)
            {
                for(int c = 0; c<8; c++)
                {
                    Position pos =new Position(r, c);
                    if(!IsEmpty(pos)) yield return pos;
                }
            }
        }
        public IEnumerable<Position> PiecePositionsFor(Player player)//местоположение фигур не своего цвета
        {
            return PiecePositions().Where(pos => this[pos].Color == player);
        }

        public bool IsInCheck(Player player)
        {
            return PiecePositionsFor(player.Opponent()).Any(pos =>
            {
                Piece piece = this[pos];
                return piece.CanCaptureOpponentKing(pos, this);
            });
        }
        public Board Copy()
        {
            Board copy = new Board();

            foreach (Position pos in PiecePositions())
            {
                copy[pos] = this[pos].Copy();
            }
            return copy;
        }
    }
}
