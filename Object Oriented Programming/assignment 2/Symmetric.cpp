#include <iostream>
#include "Symmetric.h"

Symmetric::Symmetric(int row, int col, int size) : Convol_not_Padding(row, col, size) { // BC�� ������ �����ؼ� ����
	expansion = (size - 1) / 2; // �� �𼭸� �� �ϳ��� Ȯ��� size
}

void Symmetric::Symmetric_Padding(int** arr) {
	Copy_Arr(input, arr, expansion); // input �߾ӿ� arr data ����

	for (int i = expansion; i < input_row - expansion; i++) { // column symmetric ���
		for (int j = 0; expansion - j - 1 >= 0; j++) {
			input[i][expansion - j - 1] = input[i][expansion + j]; // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
			input[i][(input_col - 1 - expansion) + j + 1] = input[i][(input_col - 1 - expansion) - j]; // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
		}
	}

	for (int i = 0; expansion - i - 1 >= 0; i++) { // row symmetric ���
		memcpy(input[expansion - i - 1], input[expansion + i], sizeof(int) * input_col); // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
		memcpy(input[(input_row - 1 - expansion) + i + 1], input[(input_row - 1 - expansion) - i], sizeof(int) * input_col); // memcpy�� �̿��� for�� ���� �� ȿ�������� data ����
	}
}

void Symmetric::Print() {
	Line(20);
	cout << "symmetric padding convolution";
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