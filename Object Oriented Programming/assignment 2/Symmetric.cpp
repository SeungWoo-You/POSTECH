#include <iostream>
#include "Symmetric.h"

Symmetric::Symmetric(int row, int col, int size) : Convol_not_Padding(row, col, size) { // BC의 생성자 지정해서 생성
	expansion = (size - 1) / 2; // 네 모서리 중 하나의 확장된 size
}

void Symmetric::Symmetric_Padding(int** arr) {
	Copy_Arr(input, arr, expansion); // input 중앙에 arr data 복사

	for (int i = expansion; i < input_row - expansion; i++) { // column symmetric 계산
		for (int j = 0; expansion - j - 1 >= 0; j++) {
			input[i][expansion - j - 1] = input[i][expansion + j]; // memcpy를 이용해 for문 보다 더 효율적으로 data 복사
			input[i][(input_col - 1 - expansion) + j + 1] = input[i][(input_col - 1 - expansion) - j]; // memcpy를 이용해 for문 보다 더 효율적으로 data 복사
		}
	}

	for (int i = 0; expansion - i - 1 >= 0; i++) { // row symmetric 계산
		memcpy(input[expansion - i - 1], input[expansion + i], sizeof(int) * input_col); // memcpy를 이용해 for문 보다 더 효율적으로 data 복사
		memcpy(input[(input_row - 1 - expansion) + i + 1], input[(input_row - 1 - expansion) - i], sizeof(int) * input_col); // memcpy를 이용해 for문 보다 더 효율적으로 data 복사
	}
}

void Symmetric::Print() {
	Line(20);
	cout << "symmetric padding convolution";
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