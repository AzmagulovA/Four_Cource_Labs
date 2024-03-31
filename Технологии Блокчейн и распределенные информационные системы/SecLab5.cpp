// Lab5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "mpi.h"
#include <cstdio>
#include <windows.h>

using namespace std;

int** GetMatrix(int n) {
    int** matrix = new int* [n];
    for (int i = 0; i < n; i++)
        matrix[i] = new int[n + 1];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n + 1; j++)
        {
            matrix[i][j] = rand() % 9 + 1;//1-9
        }

    return matrix;
}

float** GetFloatMatrix(int n, int** matrix) {
    float** res = new float* [n];
    for (int i = 0; i < n; i++)
        res[i] = new float[n + 1];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n + 1; j++)
        {
            res[i][j] = (float)matrix[i][j];
        }

    return res;
}




int** CreateEmptyMatrix(int rows, int colums) {
    int** arr = new int* [rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[colums];
        for (int j = 0; j < colums; j++) {
            arr[i][j] = 0;
        }
    }
    return arr;
}

float** CreateEmptyFloatMatrix(int rows, int colums) {
    float** arr = new float* [rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new float[colums];
        for (int j = 0; j < colums; j++) {
            arr[i][j] = 0;
        }
    }
    return arr;
}


void PrintBuf(int* matrix, int size, int rows) {
    for (int i = 0; i < size; i++) {
        cout << matrix[i] << " ";
        if (i / rows == 0) {
            cout << endl;
        }
    }
}
void PrintMatrix(int** matrix, int size) {
    cout << "Matrix: " << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size + 1; j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}
void Pryamoi(int n, float** matrix) {
    float coef;
    float values[10000];
    double beginTime, endTime;
    beginTime = MPI_Wtime();
    for (int i = 0; i < n; i++) {
        coef = matrix[i][i];//запоминаем левый верхний элемент рассматриваемой строки
        for (int j = n; j >= i; j--)//делим все элементы строки, само число становится равным 1
        {
            matrix[i][j] = matrix[i][j] / coef;
        }
        for (int j = i + 1; j < n; j++) {//рассматриваем последующие строки
            coef = matrix[j][i];//запоминаем первый значимый элемент строки
            for (int k = n; k >= i; k--)//для всех элементов строки
            {
                matrix[j][k] = matrix[j][k] - coef * matrix[i][k];//вычитаем соответсвующее значение умноженное на коэф. (таким образом первый знач эл = 0)
            }
        }
    }
    endTime = MPI_Wtime();
    std::cout << "Pryamoi Time = " << endTime - beginTime << std::endl;
}

int main(int* argc, char** argv)
{
    int numtask, rank;
    int Size = 4096;
    MPI_Init(argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);//количество процессов

    int rows;

    int rowsStart;
    int rowsEnd;
    int counter;
    float  values[10000];
    MPI_Status status;

    if (rank == 0)
    {
        int** matrix = GetMatrix(Size);
        float** PryamMatrix = GetFloatMatrix(Size, matrix);

        Pryamoi(Size, PryamMatrix);

        delete[] PryamMatrix;

       // PrintMatrix(matrix,Size);

        counter = 0;

        double beginTime, endTime;
        rows = Size / (numtask - 1);
        int* helpermas = new int[(Size + 1) * rows];
        int* buf1 = new int[(Size + 1) * Size];//начальная матрица
        float* from = new float[(Size + 1) * rows];//результат потоков
        float* res = new float[(Size + 1) * Size];//конечный результат

        beginTime = MPI_Wtime();

        for (int j = 0; j < Size; j++) {
            for (int k = 0; k < (Size + 1); k++) {
                buf1[counter] = matrix[j][k];//перенос в единый буфер
                counter++;
                //cout << buf1[counter]<<" ";

            }
            //cout << endl;
        }
        //отправка потокам их куски общей матрицы
        for (int i = 1; i < numtask; i++) {

            rowsStart = rows * (i - 1);//начало строк для  потока
            rowsEnd = (i)*rows - 1;//конец строк для потока
            counter = 0;

            for (int j = 0; j < (Size + 1) * rows; j++) {
                helpermas[counter] = buf1[j + rowsStart * (Size + 1)];//запись определенного куска
                counter++;
            }
            MPI_Send(&rowsStart, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&rowsEnd, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(helpermas, (Size + 1) * rows, MPI_FLOAT, i, 0, MPI_COMM_WORLD);

        }
        delete[] helpermas;
        counter = 0;
        //прием со всех потоков обработанные куски для формирования ответа
        for (int i = 1; i < numtask;i++) {
            MPI_Recv(from, (Size + 1) * rows, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
            for (int j = 0; j < (Size + 1) * rows;j++) {
                res[counter] = from[j];
                counter++;
            }
        }
        endTime = MPI_Wtime();
        std::cout << "Parall Time = " << endTime - beginTime << std::endl;

        delete[] buf1;
    }
    else {


        rows = Size / (numtask - 1);
        int* buf1 = new int[(Size + 1) * rows];//отсеченная версия первой матрицы

        float* send = new float[(Size + 1) * (rows * rank)];

        float* from = new float[(Size + 1) * (rows )];

        MPI_Recv(&rowsStart, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&rowsEnd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(buf1, (Size + 1) * rows, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);//получение своего куска матрицы от нулевого потока

        float** startMatrix = CreateEmptyFloatMatrix(rows, (Size + 1));
        float** fromMatrix = CreateEmptyFloatMatrix(rows, (Size + 1));
        int counter = 0;

        //матрица своего потока
        for (int j = 0; j < rows ; j++) {
            for (int k = 0; k < (Size + 1); k++) {
                startMatrix[j][k] = (float)buf1[counter];//рабочая матрица без заполнений
                counter++;
            }
        }
        counter = 0;
        float coef;
        //получаем со всех потоков куски
        for (int z = 1; z < rank;z++) {
            counter = 0;
            MPI_Recv(from, (Size + 1) * (rows), MPI_FLOAT, z, 0, MPI_COMM_WORLD, &status);//получение кусков от других потоков
            for (int j = 0; j < rows ; j++) {
                for (int k = 0; k < (Size + 1); k++) {
                    fromMatrix[j][k] = (float)from[counter];//готовая матрица с прошлого потока
                    counter++;
                }
            }
            //вычитаем со своего куска куски от других потоков
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < rows; j++) {
                    coef = startMatrix[i][j+rows*(z-1)];
                    for (int k = (Size + 1); k >= j; k--) {
                        startMatrix[i][k] -= coef * fromMatrix[j][k];
                    }
                }
            }
        }
        delete[] from;
        counter = 0;
        //работа со своим участком
            for (int i = 0; i < rows; i++) {
                coef = startMatrix[i][i + rowsStart + counter];

                for (int j = (Size + 1); j >= rowsStart + counter; j--)
                    startMatrix[i][j] /= coef;
                for (int j = i + 1; j < rows; j++) {
                    coef = startMatrix[j][rowsStart + counter];
                    for (int k = (Size + 1); k >= rowsStart + counter; k--)
                        startMatrix[j][k] -= coef * startMatrix[i][k];
                }
            }
        

        counter = 0;
        
        //отправка обработанного куска матрицы всем последующим потокам
        for (int i = rank+1; i < numtask;i++) {
            counter = 0;
            for (int j = 0; j < rows ; j++) {
                for (int k = 0; k < (Size + 1); k++) {
                    send[counter] = (float)startMatrix[j][k];//перенос в единый буфер
                    counter++;
                }
            }
            if (rank!=4)MPI_Send(send, (Size + 1)* rows, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
        if (rank==numtask-1) {//выполнение для последнего потока
            for (int j = 0; j < rows; j++) {
                for (int k = 0; k < (Size + 1); k++) {
                    send[counter] = (float)startMatrix[j][k];//перенос в единый буфер
                    counter++;
                }
            }

        }
        MPI_Send(send, (Size + 1)* rows, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);//отправка куска нулевому потоку для формирования ответа
        delete[] send;
        delete[] startMatrix;

    }
    MPI_Finalize();

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
