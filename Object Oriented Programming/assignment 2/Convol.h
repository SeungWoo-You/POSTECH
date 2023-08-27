#ifndef CONVOL_H
#define CONVOL_H
#include "AllocArr.h"
using namespace std;
// 이후 각 함수의 자세한 설명은 보고서 참고

class Convol_not_Padding {
private: 

protected: // Base Class(BC)의 변수들을 Deried Class(DC)에서도 사용해야 하므로 protected로 선언
	int kernel_size;
	int input_row;
	int input_col;
	int** kernel;
	int** input;
	int** output;

public:
	Convol_not_Padding(int row, int col, int size); // 생성자
	void Gen_Data(string type); // data 생성
	int Calc_Convol(int row_pos, int col_pos); // output 한 칸의 값을 계산
	void Calc_Output(); // ouput 전체 계산
	int Row(int** arr); // arr의 row size return
	int Col(int** arr);// arr의 column size return
	void Copy_Arr(int** from, int** to); // from에서 to로 data 복사
	void Copy_Arr(int** input, int** arr, int expansion); // input으로 arr data 복사
	int** Get_Arr(string type); // input, kernel, output 중 한 개 주소 반환
	void Print_Arr(string type); // input, kernel, output 중 한 개 출력
	void Print(); // 결과 출력
	void Line(int n); // n개 만큼 '=' 출력
	~Convol_not_Padding(); // 소멸자
};


#endif

