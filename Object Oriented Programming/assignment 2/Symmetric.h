#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "Convol.h"

class Symmetric : public Convol_not_Padding { // ��� ����
private:
	int expansion;

public:
	Symmetric(int row, int col, int size); // ������
	void Symmetric_Padding(int** arr); // padding
	void Print(); // ��� ���

};

#endif