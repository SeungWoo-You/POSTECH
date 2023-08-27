#include <iostream>
#include <cstdlib> // rand 함수 사용
#include "Convol.h"

Convol_not_Padding::Convol_not_Padding(int row, int col, int size) : input_row(row), input_col(col), kernel_size(size) {
	AllocArr A;
	
	try { // 잘못된 할당을 받을 경우(예를 들어, output의 size가 -1이 될 때) 예외 처리
		kernel = A.alloc(kernel, size, size);
		input = A.alloc(input, row, col);
		output = A.alloc(output, row - size + 1, col - size + 1);
	}

	catch (bad_alloc ex) { // bad_alloc은 allocation에 실패했을 경우이다
		cout << ex.what() << endl; // 이유 표시
		cout << "The size of row or column of matrices may be less than 0" << endl;
		exit(1); // 프로그램 강제 종료
	}
}

void Convol_not_Padding::Gen_Data(string type) {
	int** arr = Get_Arr(type);

	for (int i = 0; i < Row(arr); i++) {
		for (int j = 0; j < Col(arr); j++) {
			arr[i][j] = rand() % 10; // 생성할 랜덤 값. % 뒤의 숫자를 바꿔서 0 ~ n - 1까지로 설정할 수 있다. 지금은 0 ~ 9 상태
		}
	}
}

int Convol_not_Padding::Calc_Convol(int row_pos, int col_pos) {
	int sum = 0;

	for (int i = 0; i < kernel_size; i++) { // 계산 부분
		for (int j = 0; j < kernel_size; j++) {
			sum += input[row_pos + i][col_pos + j] * kernel[i][j];
		}
	}

	return sum;
}

void Convol_not_Padding::Calc_Output() {
	for (int i = 0; i < Row(output); i++) { // 계산 부분
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
	for (int i = 0; i < Row(from); i++) { // 계산 부분
		memcpy(to[i], from[i], sizeof(int) * Col(from)); // memcpy를 이용해 for문 보다 더 효율적으로 data 복사
	}
}

void Convol_not_Padding::Copy_Arr(int** input, int** arr, int expansion) {
	for (int i = 0; i < Row(arr); i++) { // 계산 부분
		memcpy(&(input[i + expansion][expansion]), arr[i], sizeof(int) * Col(arr)); // memcpy를 이용해 for문 보다 더 효율적으로 data 복사
	}
}

int** Convol_not_Padding::Get_Arr(string type) {
	if (type.compare("input") == 0) // input 반환
		return input;
	else if (type.compare("kernel") == 0) // kernel 반환
		return kernel;
	else // output 반환
		return output;
}

void Convol_not_Padding::Print_Arr(string type) {
	int** arr = Get_Arr(type); // 출력하고 싶은 array 불러오기

	for (int i = 0; i < Row(arr); i++) { // 출력 부분
		for (int j = 0; j < Col(arr); j++) {
			cout << arr[i][j] << ", ";
		}
		cout << endl;
	}
}

void Convol_not_Padding::Print() {
	Line(20); // kernel 출력
	cout << kernel_size << " x " << kernel_size << " kernel";
	Line(20); cout << "\n";
	Print_Arr("kernel");

	Line(20); // matrix 출력
	cout << "matrix";
	Line(20); cout << "\n";
	Print_Arr("input");

	Calc_Output(); // output 계산, 이후는 output 출력

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