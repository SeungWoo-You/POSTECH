#ifndef ALLOCARR_H
#define ALLOCARR_H

class AllocArr {
private:
	int i; // 각 함수마다 for문 용 i를 설정해주기 번거로워서
public:
	float** alloc(float** value, int row, int col); // 이차원 배열 할당
	float* alloc(float* value, int row); // overloading. 일차원 배열 할당
	void dealloc(float** value, int row); // 이차원 배열 할당 해제
	void dealloc(float* value); // overloading. 일차원 배열 할당 해제
};

#endif

