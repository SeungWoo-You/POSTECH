#ifndef ZERO_H
#define ZERO_H
#include "Convol.h"
using namespace std;

class Zero : public Convol_not_Padding { // 상속 받음
private: 
	int expansion;

public:
	Zero(int row, int col, int size); // 생성자
	void Zero_Padding(int** arr); // padding
	void Print(); // 결과 출력

};

#endif