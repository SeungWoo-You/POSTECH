#ifndef REFLECT_H
#define REFLECT_H
#include "Convol.h"
using namespace std;


class Reflect : public Convol_not_Padding { // 상속 받음
private:
	int expansion;

public:
	Reflect(int row, int col, int size); // 생성자
	void Reflect_Padding(int** arr); // padding
	void Print(); // 결과 출력
};

#endif