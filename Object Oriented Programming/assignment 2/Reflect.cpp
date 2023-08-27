#include <iostream>
#include "Reflect.h"

Reflect::Reflect(int row, int col, int size) : Convol_not_Padding(row, col, size) { // BC�� ������ �����ؼ� ����
	expansion = (size - 1) / 2; // �� �𼭸� �� �ϳ��� Ȯ��� size
}

void Reflect::Reflect_Padding(int** arr) {
	Copy_Arr(input, arr, expansion); // input �߾ӿ� arr data ����

	for (int i = expansion; i < input_row - expansion; i++) { // column reflect ���
		for (int j = 1; expansion - j >= 0; j++) {
			input[i][expansion - j] = input[i][expansion + j];
			input[i][(input_col - 1 - expansion) + j] = input[i][(input_col - 1 - expansion) - j];
		}
	}

	for (int i = 1; expansion - i >= 0; i++) { // row reflect ���
		memcpy(input[expansion - i], input[expansion + i], sizeof(int) * input_col); // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
		memcpy(input[(input_row - 1 - expansion) + i], input[(input_row - 1 - expansion) - i], sizeof(int) * input_col); // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
	}
}

void Reflect::Print() {
	Line(20);
	cout << "reflect padding convolution";
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