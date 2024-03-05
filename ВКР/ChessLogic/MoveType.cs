using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessLogic
{
    public enum MoveType
    {
        Normal,
        CastleKS,//рокировка в короткую
        CastleQS,//длинную
        DoublePawn,
        EnPassant,//взятие на проходе
        PawnPromotion//превращение пешки
    }
}
