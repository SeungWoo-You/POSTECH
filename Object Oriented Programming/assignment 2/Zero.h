#ifndef ZERO_H
#define ZERO_H
#include "Convol.h"
using namespace std;

class Zero : public Convol_not_Padding { // ��� ����
private: 
	int expansion;

public:
	Zero(int row, int col, int size); // ������
	void Zero_Padding(int** arr); // padding
	void Print(); // ��� ���

};

#endif