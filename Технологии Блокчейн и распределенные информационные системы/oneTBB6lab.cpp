// oneTBB6lab.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <tbb/tbb.h>
#include <cstdio>
#include <windows.h>

using namespace std;

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
void PrintArr(int** matrix, int rows,int* values) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < rows; j++) {
			printf(" %d ", matrix[i][j]);
		}
		printf(" %d ", values[i]);
		printf("\n");
	}
	printf("\n");
}

int** CreateFilledMatrix(int rows, int colums) {//
	int** arr = new int* [rows];
	for (int i = 0; i < rows; i++) {
		arr[i] = new int[colums];
		for (int j = 0; j < colums; j++) {
			arr[i][j] = rand() % 9 + 1;
		}
	}
	return arr;
}
int* CreateFilledRow(int Size) {//
	int* arr = new int[Size];

	for (int i = 0; i < Size;i++) {
		arr[i] = rand() % 9 + 1;
	}
	return arr;
}

int** GetMatrix(int n) {
	int** matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n ; j++)
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
int* CopyRow(int* row,int Size) {
	int* copy = new int[Size];
	for (int i = 0; i < Size;i++) {
		copy[i] = row[i];
	}
	return copy;
}

void Pryamoi(int** A, int* values, int n) {
	double* X = new double[n];

	float** res = GetFloatMatrix(n, A);

	int* PryamoiValues = CopyRow(values, n);

	auto begin = std::chrono::steady_clock::now();
	for (int k = 0; k < n; k++)
	{
			for (int j = k+1; j < n; j++) {
				float m = res[j][k] / res[k][k];
				for (int i = k; i < n; i++) {
					res[j][i] = res[j][i] - m * res[k][i];
				}
				PryamoiValues[j] = PryamoiValues[j] - m * PryamoiValues[k];
			}
	}
	delete[] PryamoiValues;
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "Pryamoi time: " << elapsed_ms.count() << " ms\n";
	
}

void Parall(int** A, int* values, int n) {
	double* X = new double[n];

	float** res = GetFloatMatrix(n,A);

	auto begin = std::chrono::steady_clock::now();
	for (int k = 0; k < n; k++)
	{
		tbb::parallel_for(tbb::blocked_range<int>(k+1, n), [&](const tbb::blocked_range<int>& range) {
			for (int j = range.begin(); j < range.end(); j++) {
				float m = res[j][k] / res[k][k];
				for (int i = k; i < n; i++) {
					res[j][i] = res[j][i] - m * res[k][i];
				}
				values[j] = values[j] - m * values[k];
			}
		});
	}
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	
	std::cout << "Parall time: " << elapsed_ms.count() << " ms\n";

}


int main()
{
	int Size = 2048;

	int** matrix = CreateFilledMatrix(Size, Size);

	int* yValues = CreateFilledRow(Size);

	//PrintArr(matrix, Size, yValues);
	Pryamoi(matrix, yValues, Size);

	Parall(matrix, yValues, Size);

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
