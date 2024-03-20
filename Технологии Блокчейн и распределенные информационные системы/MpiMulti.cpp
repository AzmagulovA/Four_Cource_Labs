// MpiMulti.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdio.h>
#include <mpi.h>
//#include <compare>
#include <chrono> 

using namespace std;



int RandomNumb(int min, int max) {
	return rand() % (max - min + 1) + min;
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
void PrintArr(int** matrix, int rows, int colums) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < colums; j++) {
			printf(" %d ",matrix[i][j]);
		}
		printf("\n");
	}
}

int** CreateFilledMatrix(int rows, int colums) {//
	int** arr = CreateEmptyMatrix(rows, colums);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < colums; j++) {
			arr[i][j] = RandomNumb(1, 10);//диапазон случайных чисел
		}
	}
	return arr;
}

int** Posled_Alg(int** matrix1, int** matrix2, int** resultMatrix,int Size) {
	auto begin = std::chrono::steady_clock::now();
	for (int i = 0; i < Size; i++) {
		for (int k = 0; k < Size; k++) {
			for (int j = 0; j < Size; j++) {
				resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	cout << "Parall Time: " << elapsed_ms.count() << " ms\n";

	return resultMatrix;
}


int main(int *argc, char* argv[])
{

	int Size = 2048;
	int rank;
	int size;
	int rows, last_rows, last_cols, rowsStart, rowsEnd;

	

	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);//номер запущенного процесса
	MPI_Comm_size(MPI_COMM_WORLD, &size);//колличество запущенных процессов
	int** matrix1 = CreateEmptyMatrix(Size, Size);
	int** matrix2 = CreateEmptyMatrix(Size, Size);
	int** resultMatrix = CreateEmptyMatrix(Size, Size);
	MPI_Status status;

	rows  = Size / (size - 1);//количество вычислений на 1 поток

	if (rank == 0)
	{
		double beginTime, endTime;

		

		resultMatrix = Posled_Alg(matrix1, matrix2, resultMatrix, Size);

		resultMatrix = CreateEmptyMatrix(Size, Size);
		beginTime = MPI_Wtime();
		

		matrix1 = CreateFilledMatrix(Size, Size);
		matrix2 = CreateFilledMatrix(Size, Size);


		resultMatrix = CreateEmptyMatrix(Size, Size);

		int *buf1 = new int[Size* Size];//отсеченная версия первой матрицы
		int *buf2 = new int[Size * Size];
		int *buf3 = new int[Size * rows];

		int *helpermas = new int[Size * rows];

		rowsStart = rank * rows;//начало для потока
		rowsEnd = (rank + 1) * rows;//конец для потока


		int counter = 0;

		for (int j = 0; j < Size; j++) {
			for (int k = 0; k < Size; k++) {
				buf1[counter] = matrix1[j][k];
				buf2[counter] = matrix2[j][k];
				counter++;
				//cout << buf1[j*Size+k]<<" ";
			}
			//cout << endl;
		}

		for (int i = 1; i < size; i++) {//разделение на потоки
			rowsStart = rows * (i - 1);//начало строк для  потока
			rowsEnd = (i)*rows - 1;
			counter = 0;
			for (int j =0; j <Size*rows; j++) {
				helpermas[counter] = buf1[j+rowsStart*Size];
				counter++;
			}
			MPI_Send(&rowsStart, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&rowsEnd, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

			MPI_Send(helpermas, Size * rows, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(buf2, Size * Size, MPI_INT, i, 0, MPI_COMM_WORLD);

		}
		for (int i = 1; i < size; i++) {//разделение на потоки
			
			MPI_Recv(buf3, Size * rows, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			rowsStart = rows * (i - 1);//начало строк для  потока
			rowsEnd = (i)*rows ;
			counter = 0;
			for (int j = rowsStart; j < rowsEnd; j++) {
				for (int k = 0; k < Size ; k++) {
					resultMatrix[j ][k] = buf3[counter];
					counter++;
				}
		    }
		 }
		endTime = MPI_Wtime();
		double time = endTime - beginTime;
		printf("Time %f s\n", time);

	}
	else {

		int counter = 0;
		int *buf1 = new int[Size * rows];
		int *buf2 = new int[Size * Size];
		int* buf3 = new int[Size * rows];
		int* helper = new int[Size * rows];;
		resultMatrix = CreateEmptyMatrix(Size, Size);
		MPI_Recv(&rowsStart, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,&status);
		MPI_Recv(&rowsEnd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);



		MPI_Recv(buf1, Size * rows, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(buf2, Size * Size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			for (int i = 0; i < rows; i++) {
				for (int k = 0; k < Size; k++) {
					for (int j = 0; j < Size; j++) {
						resultMatrix[i][j] += buf1[i * Size + k] * buf2[k * Size + j];
					}
				}
			}
			
			for (int i = 0; i < rows;i++) {
				for (int j = 0; j < Size; j++) {
					buf3[i * Size + j] = resultMatrix[i][j];//заполнение буфера
				}
			}
			MPI_Send(buf3, Size* rows, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
	}

	MPI_Finalize();
	
	return 0;

}


