
using System.Data;

static void From_FEN_To_Mass(string fenstr)
{
    char[,] FENMass = new char[8,8];
    for (int i = 0; i < FENMass.GetLength(0); i++)
    {
        for (int j = 0; j < FENMass.GetLength(1); j++)
        {
            FENMass[i, j] = Convert.ToChar(Convert.ToString(i+1));
        }
    }
    int counter = 0;
    int slashcounter = 0;
    int massrowcounter = 0;
    string forFirstMove;
    while (fenstr[counter]!=' ')//пока не столкнулись с пробелом работаем
    {
        if (fenstr[counter] == '/') //если встретили перевод строки, то заполняем новую строку
        { 
           slashcounter++;//запомнили переводимую горизонталь
        }
        else
        {
            if (Char.IsDigit(fenstr[counter]))//если натолкнулись на число
            {
                massrowcounter+= Convert.ToInt32(fenstr[counter].ToString());//то следующий заполняемый индекс будет = текущий + число
            }
            else
            {                
                FENMass[slashcounter, massrowcounter % 8] = fenstr[counter];//заполняем индекс
                massrowcounter++;
            }
        }
        counter++;
    }
    for (int i = 0; i < FENMass.GetLength(0); i++)//вывод массива
    {
        for (int j = 0; j < FENMass.GetLength(1); j++)
        {
            Console.Write(FENMass[i, j] + " ");
        }
        Console.WriteLine();
    }   
    Console.WriteLine($"First Move: {fenstr[counter+1]}");
    forFirstMove = fenstr.Substring(counter + 2);
    Console.WriteLine(forFirstMove);
    if (forFirstMove.Contains('k'))
    {
        Console.WriteLine("Чёрный король может делать короткую рокировку");
    }
    if (forFirstMove.Contains('q'))
    {
        Console.WriteLine("Чёрный король может делать длинную рокировку");
    }
    if (forFirstMove.Contains('K'))
    {
        Console.WriteLine("Белый король может делать короткую рокировку");
    }
    if (forFirstMove.Contains('Q'))
    {
        Console.WriteLine("Белый король может делать длинную рокировку");
    }
    Console.WriteLine();      
}
while (true) {
    
    string FEN = Console.ReadLine();
   // string startFEN = "rnbQkbnr/ppp2ppp/8/4p3/4P3/8/PPP2PPP/RNB1KBNR b KQkq - 0 4";
   // string[] FENMass = new string[8] { "rnbqkbnr", "pppppppp", "8", "8", "8", "8", "PPPPPPPP", "RNBQKBNR" };
    From_FEN_To_Mass(FEN);    
}






