#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "Convol.h"

class Symmetric : public Convol_not_Padding { // 상속 받음
private:
	int expansion;

public:
	Symmetric(int row, int col, int size); // 생성자
	void Symmetric_Padding(int** arr); // padding
	void Print(); // 결과 출력

};

#endif