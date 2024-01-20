using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pyatnash
{
    
        class Matrix : ICloneable
        {
            public int Differences;//количество несовпавших клеток, чем меньше,тем лучше
            public int Score;//differences+step 
            public int Step = 0;//текущий шаг
            public Matrix PastMatrix = null;//указатель на прошлую матрицу(из какой получилась эта)
            public bool CheckPoint = false;//индикатор того что мы уже смотрели эту матрицу
            public int[,] Position = new int[3, 3]      { { 2, 1, 6 } ,
                                                          { 4, 9, 8 },
                                                          { 7, 5, 3 } };
            public int[,] TargetPosition = new int[3, 3] { { 1, 2, 3 } ,
                                                           { 8, 9, 4 },
                                                           { 7, 6, 5 } };


            public object Clone()
            {
                return MemberwiseClone();
            }
            public void PrintStep()
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (Position[i, j] == 9) Console.Write("- ");
                        else Console.Write(Position[i, j] + " ");
                    }
                    Console.WriteLine();
                }

                Console.Write($"Differences:{Differences} ");
                Console.Write($"Step:{Step} ");
                Console.Write($"Score:{Score} ");
                Console.WriteLine("");
                Console.Write("_______________");
                Console.WriteLine("");
            }
            public bool CompareOfPosition(Matrix second)//сравнение матриц 
            {
            
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (this.Position[i, j] != second.Position[i, j])
                        {
                            return false;
                        }
                    }
                }
                return true;
            
            }
            public string SearchNine()
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (this.Position[i, j] == 9)
                        {
                            return i.ToString() + j.ToString();
                        }
                    }
                }
                return "none";
            }

            public Matrix SwapNine(string ninePosition, string needNinePosition)
            {
                Matrix result = new();
                
                result.Position = (int[,])this.Position.Clone();
                

                int swapNumber = this.Position[int.Parse(needNinePosition[0].ToString()), int.Parse(needNinePosition[1].ToString())];
                result.Position[int.Parse(ninePosition[0].ToString()), int.Parse(ninePosition[1].ToString())] = swapNumber;
                result.Position[int.Parse(needNinePosition[0].ToString()), int.Parse(needNinePosition[1].ToString())] = 9;

                result.Differences = result.SetDifferences();
                result.Step = this.Step + 1;
                result.Score = result.Differences + result.Step;
                result.PastMatrix = this;

                return result;

            }
            public int SetDifferences()
            {
                int result = 0;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (this.Position[i, j] != this.TargetPosition[i, j]) result++;
                    }
                }
                return result;
            }
            public List<Matrix> NewStep()
            {
                string helper = SearchNine();
                List<Matrix> result = new List<Matrix>();
                Matrix first;
                Matrix second;
                Matrix third;
                Matrix four;
                this.CheckPoint = true;
                switch (helper)
                {
                    case "00":
                        first = SwapNine("00", "01");
                        second = SwapNine("00", "10");
                        result.Add(first);
                        result.Add(second);
                        return result;

                    case "01":
                        first = SwapNine("01", "00");
                        second = SwapNine("01", "02");
                        third = SwapNine("01", "11");
                        result.Add(first);
                        result.Add(second);
                        result.Add(third);
                        return result;

                    case "02":
                        first = SwapNine("02", "01");
                        second = SwapNine("02", "12");
                        result.Add(first);
                        result.Add(second);
                        return result;

                    case "10":
                        first = SwapNine("10", "00");
                        second = SwapNine("10", "11");
                        third = SwapNine("10", "20");
                        result.Add(first);
                        result.Add(second);
                        result.Add(third);
                        return result;

                    case "11":
                        first = SwapNine("11", "10");
                        second = SwapNine("11", "01");
                        third = SwapNine("11", "12");
                        four = SwapNine("11", "21");
                        result.Add(first);
                        result.Add(second);
                        result.Add(third);
                        result.Add(four);
                        return result;

                    case "12":
                        first = SwapNine("12", "02");
                        second = SwapNine("12", "11");
                        third = SwapNine("12", "22");
                        result.Add(first);
                        result.Add(second);
                        result.Add(third);
                        return result;

                    case "20":
                        first = SwapNine("20", "21");
                        second = SwapNine("20", "10");
                        result.Add(first);
                        result.Add(second);
                        return result;

                    case "21":
                        first = SwapNine("21", "11");
                        second = SwapNine("21", "20");
                        third = SwapNine("21", "22");
                        result.Add(first);
                        result.Add(second);
                        result.Add(third);
                        return result;

                    case "22":
                        first = SwapNine("22", "12");
                        second = SwapNine("22", "21");
                        result.Add(first);
                        result.Add(second);
                        return result;

                    default:
                        return null;
                }

            }
        }
    }


