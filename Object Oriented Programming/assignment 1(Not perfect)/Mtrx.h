#ifndef MTRX_H
#define MTRX_H

class Mtrx {
private:

public:
	int row(float** arr); // arr의 row size 계산해서 반환
	int col(float** arr); // arr의 col size 계산해서 반환
	int size(float* arr); // arr의 size 계산해서 반환
	float** calc_matrix(float** front, float** rear, float** output); // 이차원 배열 x 이차원 배열 행렬곱 계산 후 결과 반환
	float* calc_matrix(float* front, float** rear, float* output);// overloading. 일차원 배열 x 이차원 배열 행렬곱 계산 후 결과 반환
	float** transpose(float** arr); // 전치행렬 계산 후 반환
};


#endif
