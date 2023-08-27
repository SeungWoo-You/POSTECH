#include <iostream>
#include <ctime> // random seed ����
#include "Convol.h"
#include "Zero.h"
#include "Reflect.h"
#include "Symmetric.h"
using namespace std;

int main() {
	int row = 0, col = 0, size = 0; // row: input row size		col: input column size		size: kernel row & column size

	srand((unsigned int)time(NULL));

	cout << "Input the size of row of input: ";
	cin >> row;
	cout << "Input the size of column of input: ";
	cin >> col;

	while (1) {
		try { // kernel size�� ¦���� �Է��� ��� ���� ó��
			cout << "Input the size of row of kernel: ";
			cin >> size;

			if (size % 2 == 0)
				throw size;
		}

		catch (int) {
			cout << "error: the size of row of kernel must be odd." << endl;
			cin.ignore(256, '\n');
			continue;
		}

		Convol_not_Padding npad(row, col, size); // not paddding ��ü ����
		npad.Gen_Data("input"); // ��ü input�� random data ����
		npad.Gen_Data("kernel");// ��ü output�� random data ����

		npad.Print(); // ��� ���

		row += size - 1; // 3���� padding�� input row�� row + size - 1�� ũ�⸦ ������ �Ѵ�
		col += size - 1; // 3���� padding�� input col�� col + size - 1�� ũ�⸦ ������ �Ѵ�
		Zero zpad(row, col, size); // zero padding ��ü ����
		zpad.Copy_Arr(npad.Get_Arr("kernel"), zpad.Get_Arr("kernel")); // npad�� ���� kernel �����ؿ���
		zpad.Zero_Padding(npad.Get_Arr("input")); // zero padding ���
		zpad.Print(); // ��� ���

		Reflect rpad(row, col, size); // reflect padding ��ü ����
		rpad.Copy_Arr(npad.Get_Arr("kernel"), rpad.Get_Arr("kernel")); // npad�� ���� kernel �����ؿ���
		rpad.Reflect_Padding(npad.Get_Arr("input")); // reflect padding ���
		rpad.Print(); // ��� ���

		Symmetric spad(row, col, size); // symmetric padding ��ü ����
		spad.Copy_Arr(npad.Get_Arr("kernel"), spad.Get_Arr("kernel")); // npad�� ���� kernel �����ؿ���
		spad.Symmetric_Padding(npad.Get_Arr("input")); // symmetric padding ���
		spad.Print(); // ��� ���

		string answer = "";
		while (1) { // ��� ���� �亯�� �߸� �Է����� �� ���� ó��
			cout << "Continue?(Y/N): ";
			cin >> answer;

			if (answer.compare("Y") == 0 || answer.compare("N") == 0)
				break;

			cout << "Wrong asnwer"<<endl;
			cin.ignore(256, '\n');
			answer = "";
			continue;
		}

		if (answer.compare("Y") == 0) { // kernel�� input�� ���� �޾ƾ� �ϹǷ� row�� col ũ�⸦ ������� �ǵ���
			row -= size - 1;
			col -= size - 1;
			continue;
		}
		else
			break;
	} // continue�� �ϴ� break�� �ϴ�, ������ ������ 4���� ��ü(npad, zpad, rpad, spad)�� while�� ���ο��� ���������Ƿ� ������ ���ÿ� �Ҹ��Ѵ�. ��, continue�� ���� ��ü �Ҹ� �� ���� ��ü�� �����ϰ� �ǰ�, break�� ���� ��ü�� �Ҹ�ȴ�

	return 0;
}
