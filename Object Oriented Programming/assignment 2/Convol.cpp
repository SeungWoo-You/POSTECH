#include <iostream>
#include <cstdlib> // rand �Լ� ���
#include "Convol.h"

Convol_not_Padding::Convol_not_Padding(int row, int col, int size) : input_row(row), input_col(col), kernel_size(size) {
	AllocArr A;
	
	try { // �߸��� �Ҵ��� ���� ���(���� ���, output�� size�� -1�� �� ��) ���� ó��
		kernel = A.alloc(kernel, size, size);
		input = A.alloc(input, row, col);
		output = A.alloc(output, row - size + 1, col - size + 1);
	}

	catch (bad_alloc ex) { // bad_alloc�� allocation�� �������� ����̴�
		cout << ex.what() << endl; // ���� ǥ��
		cout << "The size of row or column of matrices may be less than 0" << endl;
		exit(1); // ���α׷� ���� ����
	}
}

void Convol_not_Padding::Gen_Data(string type) {
	int** arr = Get_Arr(type);

	for (int i = 0; i < Row(arr); i++) {
		for (int j = 0; j < Col(arr); j++) {
			arr[i][j] = rand() % 10; // ������ ���� ��. % ���� ���ڸ� �ٲ㼭 0 ~ n - 1������ ������ �� �ִ�. ������ 0 ~ 9 ����
		}
	}
}

int Convol_not_Padding::Calc_Convol(int row_pos, int col_pos) {
	int sum = 0;

	for (int i = 0; i < kernel_size; i++) { // ��� �κ�
		for (int j = 0; j < kernel_size; j++) {
			sum += input[row_pos + i][col_pos + j] * kernel[i][j];
		}
	}

	return sum;
}

void Convol_not_Padding::Calc_Output() {
	for (int i = 0; i < Row(output); i++) { // ��� �κ�
		for (int j = 0; j < Col(output); j++) {
			output[i][j] = Calc_Convol(i, j);
		}
	}
}

int Convol_not_Padding::Row(int** arr) {
	return  _msize(arr) / sizeof(int*);
}

int Convol_not_Padding::Col(int** arr) {
	return _msize(arr[0]) / sizeof(int);
}

void Convol_not_Padding::Copy_Arr(int** from, int** to) {
	for (int i = 0; i < Row(from); i++) { // ��� �κ�
		memcpy(to[i], from[i], sizeof(int) * Col(from)); // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
	}
}

void Convol_not_Padding::Copy_Arr(int** input, int** arr, int expansion) {
	for (int i = 0; i < Row(arr); i++) { // ��� �κ�
		memcpy(&(input[i + expansion][expansion]), arr[i], sizeof(int) * Col(arr)); // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
	}
}

int** Convol_not_Padding::Get_Arr(string type) {
	if (type.compare("input") == 0) // input ��ȯ
		return input;
	else if (type.compare("kernel") == 0) // kernel ��ȯ
		return kernel;
	else // output ��ȯ
		return output;
}

void Convol_not_Padding::Print_Arr(string type) {
	int** arr = Get_Arr(type); // ����ϰ� ���� array �ҷ�����

	for (int i = 0; i < Row(arr); i++) { // ��� �κ�
		for (int j = 0; j < Col(arr); j++) {
			cout << arr[i][j] << ", ";
		}
		cout << endl;
	}
}

void Convol_not_Padding::Print() {
	Line(20); // kernel ���
	cout << kernel_size << " x " << kernel_size << " kernel";
	Line(20); cout << "\n";
	Print_Arr("kernel");

	Line(20); // matrix ���
	cout << "matrix";
	Line(20); cout << "\n";
	Print_Arr("input");

	Calc_Output(); // output ���, ���Ĵ� output ���

	Line(20);
	cout << "result";
	Line(20); cout << "\n";
	Print_Arr("output");
}

void Convol_not_Padding::Line(int n) {
	for (int i = 0; i < n; i++) {
		cout << "=";
	}
}

Convol_not_Padding::~Convol_not_Padding() {
	AllocArr A;

	A.dealloc(input, Row(input));
	A.dealloc(output, Row(output));
	A.dealloc(kernel, Row(kernel));
}