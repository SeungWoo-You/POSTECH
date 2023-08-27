#include <iostream>
#include <string>
#include "Mlp.h"

float** AllocArr::alloc(float** value, int row, int col) {
	value = new float* [row];

	for (i = 0; i < row; i++) {
		value[i] = new float [col];
	}
	
	return value;
}

float* AllocArr::alloc(float* value, int row) {
	value = new float[row];

	return value;
}

void AllocArr::dealloc(float** value, int row) {
	for (i = 0; i < row; ++i) {
		delete[] value[i];
	}

	delete[] value;
}

void AllocArr::dealloc(float* value) {
	delete[] value;
}

