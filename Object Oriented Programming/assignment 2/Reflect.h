#ifndef REFLECT_H
#define REFLECT_H
#include "Convol.h"
using namespace std;


class Reflect : public Convol_not_Padding { // ��� ����
private:
	int expansion;

public:
	Reflect(int row, int col, int size); // ������
	void Reflect_Padding(int** arr); // padding
	void Print(); // ��� ���
};

#endif