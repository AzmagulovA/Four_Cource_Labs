﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public class EnPassant:Move
    {
        public override MoveType Type => MoveType.EnPassant;
        public override Position ToPos { get; }
        public override Position FromPos { get; }
        private readonly Position capturePos;
        public EnPassant(Position from, Position to)
        {
            ToPos = to;
            FromPos = from;
            capturePos = new Position(from.Row, to.Column);
        }
        public override void Execute(Board board)
        {
            new NormalMove(FromPos,ToPos).Execute(board);
            board[capturePos] = null;
        }
    }
}
