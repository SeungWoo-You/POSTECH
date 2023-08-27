#include <iostream>
#include <string>
#include "Mlp.h"

int Mtrx::row(float** arr) {
	return  _msize(arr) / sizeof(float*);
}

int Mtrx::col(float** arr) {
	return _msize(arr[0]) / sizeof(float);
}

int Mtrx::size(float* arr) {
	return _msize(arr) / sizeof(float*);
}

void Mtrx::calc_matrix(float** front, float** rear, float** output) {
	AllocArr arr;

	if (this->col(front) != this->row(rear)) {
		cout << "Mtrx multiple error!" << endl;
		return;
	}

	int row = this->row(front);
	int col = this->col(rear);

	arr.alloc(output, row, col);

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float sum = 0;
			for (int k = 0; k < this->row(rear); k++) {
				sum += front[i][k] * rear[k][j];
			}
			output[i][j] = sum;
		}
	}
}

void Mtrx::calc_matrix(float* front, float** rear, float* output) {
	AllocArr arr;

	if (size(front) != this->row(rear)) {
		cout << "Mtrx multiple error!" << endl;
		return;
	}

	int row = size(front);
	int col = this->col(rear);

	arr.alloc(output, this->col(rear));

	
	for (int j = 0; j < col; j++) {
		float sum = 0;
		for (int k = 0; k < this->row(rear); k++) {
			sum += front[k] * rear[k][j];
		}
		output[j] = sum;
	}
}