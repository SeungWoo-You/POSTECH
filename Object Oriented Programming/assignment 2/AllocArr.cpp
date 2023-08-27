#include "AllocArr.h"

int** AllocArr::alloc(int** value, int row, int col) {
	value = new int* [row];

	for (int i = 0; i < row; i++) {
		value[i] = new int[col];
	}

	return value;
}

void AllocArr::dealloc(int** value, int row) {
	for (int i = 0; i < row; ++i) {
		delete[] value[i];
	}

	delete[] value;
}

