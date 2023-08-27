#ifndef CONVOL_H
#define CONVOL_H
#include "AllocArr.h"
using namespace std;
// ���� �� �Լ��� �ڼ��� ������ ���� ����

class Convol_not_Padding {
private: 

protected: // Base Class(BC)�� �������� Deried Class(DC)������ ����ؾ� �ϹǷ� protected�� ����
	int kernel_size;
	int input_row;
	int input_col;
	int** kernel;
	int** input;
	int** output;

public:
	Convol_not_Padding(int row, int col, int size); // ������
	void Gen_Data(string type); // data ����
	int Calc_Convol(int row_pos, int col_pos); // output �� ĭ�� ���� ���
	void Calc_Output(); // ouput ��ü ���
	int Row(int** arr); // arr�� row size return
	int Col(int** arr);// arr�� column size return
	void Copy_Arr(int** from, int** to); // from���� to�� data ����
	void Copy_Arr(int** input, int** arr, int expansion); // input���� arr data ����
	int** Get_Arr(string type); // input, kernel, output �� �� �� �ּ� ��ȯ
	void Print_Arr(string type); // input, kernel, output �� �� �� ���
	void Print(); // ��� ���
	void Line(int n); // n�� ��ŭ '=' ���
	~Convol_not_Padding(); // �Ҹ���
};


#endif

