using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessAnalyser
{
    //класс для хранения информации о игроках и цвете победившей стороны
    public enum Player//enum - перечисление
    {
        None,//значение в случае ничьи
        White,
        Black

    }
    public static class PlayerExtension 
    { 
        public static Player Opponent(this Player player)//принимает игрока и возвращает его оппонента (передача хода)
        {
            switch(player)
            {
                case Player.White:
                    return Player.Black;
                case Player.Black:
                    return Player.White;
                default:
                    return Player.None;


            }
        }

    }

}
