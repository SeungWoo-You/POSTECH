#ifndef ALLOCARR_H
#define ALLOCARR_H

class AllocArr {
private:
	int i; // �� �Լ����� for�� �� i�� �������ֱ� ���ŷο���
public:
	float** alloc(float** value, int row, int col); // ������ �迭 �Ҵ�
	float* alloc(float* value, int row); // overloading. ������ �迭 �Ҵ�
	void dealloc(float** value, int row); // ������ �迭 �Ҵ� ����
	void dealloc(float* value); // overloading. ������ �迭 �Ҵ� ����
};

#endif

