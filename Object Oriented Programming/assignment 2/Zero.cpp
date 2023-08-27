#include <iostream>
#include "Zero.h"

Zero::Zero(int row, int col, int size) : Convol_not_Padding(row, col, size) { // BC의 생성자 지정해서 생성
	expansion = (size - 1) / 2; // 네 모서리 중 하나의 확장된 size
}

void Zero::Zero_Padding(int** arr) {
	for (int i = 0; i < input_row; i++) { // 모든 값 0으로 초기화
		memset(input[i], 0, sizeof(int) * input_col);
	}

	Copy_Arr(input, arr, expansion); // input 중앙에 arr data 복사
}

void Zero::Print() {
	Line(20);
	cout << "zero padding convolution";
	Line(20); cout << "\n";

	Line(10); // input 출력
	cout << "padded matrix";
	Line(10); cout << "\n";
	Print_Arr("input");
	
	Calc_Output(); // output 계산

	Line(10); // output 출력
	cout << "result";
	Line(10); cout << "\n";
	Print_Arr("output");
}
