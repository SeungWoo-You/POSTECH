#include <iostream>
#include "Zero.h"

Zero::Zero(int row, int col, int size) : Convol_not_Padding(row, col, size) { // BC�� ������ �����ؼ� ����
	expansion = (size - 1) / 2; // �� �𼭸� �� �ϳ��� Ȯ��� size
}

void Zero::Zero_Padding(int** arr) {
	for (int i = 0; i < input_row; i++) { // ��� �� 0���� �ʱ�ȭ
		memset(input[i], 0, sizeof(int) * input_col);
	}

	Copy_Arr(input, arr, expansion); // input �߾ӿ� arr data ����
}

void Zero::Print() {
	Line(20);
	cout << "zero padding convolution";
	Line(20); cout << "\n";

	Line(10); // input ���
	cout << "padded matrix";
	Line(10); cout << "\n";
	Print_Arr("input");
	
	Calc_Output(); // output ���

	Line(10); // output ���
	cout << "result";
	Line(10); cout << "\n";
	Print_Arr("output");
}
