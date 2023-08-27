#include <iostream>
#include <ctime> // random seed 제공
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
		try { // kernel size에 짝수를 입력할 경우 예외 처리
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

		Convol_not_Padding npad(row, col, size); // not paddding 객체 생성
		npad.Gen_Data("input"); // 객체 input에 random data 생성
		npad.Gen_Data("kernel");// 객체 output에 random data 생성

		npad.Print(); // 결과 출력

		row += size - 1; // 3가지 padding의 input row는 row + size - 1의 크기를 가져야 한다
		col += size - 1; // 3가지 padding의 input col은 col + size - 1의 크기를 가져야 한다
		Zero zpad(row, col, size); // zero padding 객체 생성
		zpad.Copy_Arr(npad.Get_Arr("kernel"), zpad.Get_Arr("kernel")); // npad와 같은 kernel 복사해오기
		zpad.Zero_Padding(npad.Get_Arr("input")); // zero padding 계산
		zpad.Print(); // 결과 출력

		Reflect rpad(row, col, size); // reflect padding 객체 생성
		rpad.Copy_Arr(npad.Get_Arr("kernel"), rpad.Get_Arr("kernel")); // npad와 같은 kernel 복사해오기
		rpad.Reflect_Padding(npad.Get_Arr("input")); // reflect padding 계산
		rpad.Print(); // 결과 출력

		Symmetric spad(row, col, size); // symmetric padding 객체 생성
		spad.Copy_Arr(npad.Get_Arr("kernel"), spad.Get_Arr("kernel")); // npad와 같은 kernel 복사해오기
		spad.Symmetric_Padding(npad.Get_Arr("input")); // symmetric padding 계산
		spad.Print(); // 결과 출력

		string answer = "";
		while (1) { // 계속 여부 답변을 잘못 입력했을 때 예외 처리
			cout << "Continue?(Y/N): ";
			cin >> answer;

			if (answer.compare("Y") == 0 || answer.compare("N") == 0)
				break;

			cout << "Wrong asnwer"<<endl;
			cin.ignore(256, '\n');
			answer = "";
			continue;
		}

		if (answer.compare("Y") == 0) { // kernel과 input을 새로 받아야 하므로 row와 col 크기를 원래대로 되돌림
			row -= size - 1;
			col -= size - 1;
			continue;
		}
		else
			break;
	} // continue를 하던 break를 하던, 위에서 생성한 4개의 객체(npad, zpad, rpad, spad)는 while문 내부에서 선언했으므로 끝남과 동시에 소멸한다. 즉, continue시 기존 객체 소멸 후 새로 객체를 설정하게 되고, break시 기존 객체는 소멸된다

	return 0;
}
