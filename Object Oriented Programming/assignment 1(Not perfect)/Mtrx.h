#ifndef MTRX_H
#define MTRX_H

class Mtrx {
private:

public:
	int row(float** arr); // arr�� row size ����ؼ� ��ȯ
	int col(float** arr); // arr�� col size ����ؼ� ��ȯ
	int size(float* arr); // arr�� size ����ؼ� ��ȯ
	float** calc_matrix(float** front, float** rear, float** output); // ������ �迭 x ������ �迭 ��İ� ��� �� ��� ��ȯ
	float* calc_matrix(float* front, float** rear, float* output);// overloading. ������ �迭 x ������ �迭 ��İ� ��� �� ��� ��ȯ
	float** transpose(float** arr); // ��ġ��� ��� �� ��ȯ
};


#endif
