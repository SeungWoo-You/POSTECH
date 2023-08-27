#include <iostream>
#include <string>
#include "Mlp.h"

int Mtrx::row(float** arr) {
	return  _msize(arr) / sizeof(float*); // �޸𸮸� �Ҵ�޾� ����ϹǷ� heap������ �޸� �Ҵ� ũ�⸦ �˷��ִ� _msize ���
}

int Mtrx::col(float** arr) {
	return _msize(arr[0]) / sizeof(float);
}

int Mtrx::size(float* arr) {
	return _msize(arr) / sizeof(float);
}

float** Mtrx::calc_matrix(float** front, float** rear, float** output) {
	AllocArr A;

	if (this->col(front) != this->row(rear)) { // �� ����� ���� ������ ������Ű�� ���� ���
		cout << "Mtrx multiple error!" << endl;
		return output;
	}

	int row = this->row(front); // this �����ͷ� �Լ� row�� ����
	int col = this->col(rear);

	output = A.alloc(output, row, col);

	for (int i = 0; i < row; i++) { // ��� �� ���
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

	if (size(front) != this->row(rear)) { // �� ����� ���� ������ ������Ű�� ���� ���
		cout << "Mtrx multiple error!" << endl;
		return output;
	}

	int col = this->col(rear);

	output = A.alloc(output, this->col(rear));

	for (int j = 0; j < col; j++) { // ��� �� ���
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

	for (int i = 0; i < row; i++) { // ��ġ��� ���
		for (int j = 0; j < col; j++) {
			if(tns_arr != NULL)
				tns_arr[j][i] = arr[i][j];
		}
	}

	return tns_arr;
}