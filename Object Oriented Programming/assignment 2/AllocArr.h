#ifndef ALLOCARR_H
#define ALLOCARR_H

class AllocArr {
public:
	int** alloc(int** value, int row, int col); // 이차원 배열 할당
	void dealloc(int** value, int row); // 이차원 배열 할당 해제
};

#endif