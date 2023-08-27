#include <iostream>
#include <string>
#include "Mlp.h"

int Mtrx::row(float** arr) {
	return  _msize(arr) / sizeof(float*); // 메모리를 할당받아 사용하므로 heap공간의 메모리 할당 크기를 알려주는 _msize 사용
}

int Mtrx::col(float** arr) {
	return _msize(arr[0]) / sizeof(float);
}

int Mtrx::size(float* arr) {
	return _msize(arr) / sizeof(float);
}

float** Mtrx::calc_matrix(float** front, float** rear, float** output) {
	AllocArr A;

	if (this->col(front) != this->row(rear)) { // 두 행렬의 곱셈 조건을 만족시키지 못할 경우
		cout << "Mtrx multiple error!" << endl;
		return output;
	}

	int row = this->row(front); // this 포인터로 함수 row에 접근
	int col = this->col(rear);

	output = A.alloc(output, row, col);

	for (int i = 0; i < row; i++) { // 행렬 곱 계산
		for (int j = 0; j < col; j++) {
			float sum = 0;
			for (int k = 0; k < this->row(rear); k++) {
				sum += front[i][k] * rear[k][j];
			}
			output[i][j] = sum;
		}
	}

	return output;
}

float* Mtrx::calc_matrix(float* front, float** rear, float* output) {
	AllocArr A;

	if (size(front) != this->row(rear)) { // 두 행렬의 곱셈 조건을 만족시키지 못할 경우
		cout << "Mtrx multiple error!" << endl;
		return output;
	}

	int col = this->col(rear);

	output = A.alloc(output, this->col(rear));

	for (int j = 0; j < col; j++) { // 행렬 곱 계산
		float sum = 0;
		for (int k = 0; k < this->row(rear); k++) {
			sum += front[k] * rear[k][j];
		}
		output[j] = sum;
	}

	return output;
}

float** Mtrx::transpose(float** arr) {
	int row = this->row(arr);
	int col = this->col(arr);
	float** tns_arr = NULL;
	AllocArr A;

	tns_arr = A.alloc(tns_arr, col, row);

	for (int i = 0; i < row; i++) { // 전치행렬 계산
		for (int j = 0; j < col; j++) {
			if(tns_arr != NULL)
				tns_arr[j][i] = arr[i][j];
		}
	}

	return tns_arr;
}