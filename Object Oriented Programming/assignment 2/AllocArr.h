#ifndef ALLOCARR_H
#define ALLOCARR_H

class AllocArr {
public:
	int** alloc(int** value, int row, int col); // ������ �迭 �Ҵ�
	void dealloc(int** value, int row); // ������ �迭 �Ҵ� ����
};

#endif