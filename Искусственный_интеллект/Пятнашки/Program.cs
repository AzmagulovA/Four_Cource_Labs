// See https://aka.ms/new-console-template for more information
using Pyatnash;

class Program
{
    public static Matrix MinScoreMatrix(List<Matrix> x)//поиск минимального t для ее дальнейшей обработки
    {
        
        int helper = x.Max(x=>x.Score);
        Matrix result = x[0];
        foreach (var matrix in x)
        {
            if (!matrix.CheckPoint)//если матрица не была осмотрена
            {
                if (matrix.Score < helper)
                {
                    helper = matrix.Score;
                    result = matrix;
                }
            }
        }
        
        return result;
      //  return x.MinBy(item => item.Score);
    }


    static void Main(string[] args)
    {
        Matrix nextmatr = new();
        List<Matrix> allWays = new();//все исходы
        List<Matrix> condidateWays;//возможные ходы текущего шага

        bool find = false;//идентефикатор того что достигнута цель
        bool check = false;// идентефикатор, что уже присутствует такая позиция

        List<Matrix> result = new();

        int best = 0;//лучшие вершины
        while (!find)
        {
            condidateWays = nextmatr.NewStep();
            foreach (var choices in condidateWays)//добавление уникальных матриц в общий список
            {
                foreach (var tree in allWays)
                {
                    if (choices.CompareOfPosition(tree))//если есть такая матрица
                    {
                        check = true;
                        break;
                    }

                }
                if (!check) allWays.Add(choices);
                check = false;
            }
            nextmatr = MinScoreMatrix(allWays);//выбор матрицы с наименьшим значением

            best++;
            result.Add(nextmatr);
            if (nextmatr.Differences == 0)
            {
                find = true;
            }
        }

        /*foreach (var matrix in result)
        {
            matrix.print();
        }*/
        Matrix lastItem = result.Last();
        List<Matrix> finalres = new List<Matrix>();


        while (lastItem.PastMatrix != null)
        {
            finalres.Insert(0, lastItem);
            lastItem = lastItem.PastMatrix;

        }
        int k = 0;
        string traectoria = "";
        foreach (var matrix in finalres)
        {
            foreach (var matrixh in allWays)
            {
                k++;
                if (matrix == matrixh)
                {
                    traectoria = traectoria + " " + k.ToString();
                    k = 0;
                    break;
                }
            }

        }
        // Console.WriteLine (result.Count());


        int shirina = 0;
        int helper = 0;
        for (int i = 0; i < finalres.Count; i++)
        {
            foreach (var matrix in allWays)
            {
                if (matrix.Step == i) helper++;
            }
            if (shirina < helper) shirina = helper;
            helper = 0;
        }
        Console.WriteLine($"Просмотрено: {allWays.Count}");
        Console.WriteLine($"Лучшие: {best}");
        Console.WriteLine($"Ширина: {shirina}");
        Console.WriteLine($"Глубина: 1");
        Console.WriteLine($"Оптимальная траектория не считая начальное положения {traectoria}");
        Console.WriteLine($"Количество оптимальных вершин: {finalres.Count}");

        foreach (var matrix in finalres)
        {
            matrix.PrintStep();
        }
        Console.ReadLine();
    }
}
