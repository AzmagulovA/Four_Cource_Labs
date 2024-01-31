﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public class Knight : Piece
    {
        public override PieceType Type => PieceType.Knight;//override - переназначенный тип базового класса
        public override Player Color { get; }
        public Knight(Player color)
        {
            Color = color;

        }
        public override Piece Copy()
        {
            Knight copy = new Knight(Color);
            copy.HasMoved = HasMoved;
            return copy;
        }
        private static IEnumerable<Position> PotentialToPositions(Position from)
        {
            foreach (Direction vDir in new Direction[] { Direction.North, Direction.South })
            {
                foreach (Direction hDir in new Direction[] { Direction.East, Direction.West })
                {
                    yield return from + 2 * vDir + hDir;
                    yield return from + vDir + 2 * hDir;

                }
            }

        }
        private IEnumerable<Position> MovePositions(Position from,Board board)
        {
            return PotentialToPositions(from).Where(pos => Board.IsInside(pos)
                && (board.IsEmpty(pos) || board[pos].Color != Color));
        }
        public override IEnumerable<Move> GetMoves(Position from,Board board)
        {
            return MovePositions(from, board).Select(to => new NormalMove(from, to));
        }

    }
}
