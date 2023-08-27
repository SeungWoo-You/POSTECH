#include <iostream>
#include <string>
#include "Mlp.h"

Loss::Loss() {
	Logit = NULL;
	Label = NULL;
}

float Loss::forward(float** Logit, float** Label) {
	this->Logit = Logit; // 각 정보 private 변수에 저장
	this->Label = Label;

	Mtrx M;
	float loss = 0, sum = 0, mean = 0;

	for (int i = 0; i < M.row(Label); i++) {
		for (int j = 0; j < M.col(Label); j++) {
			sum += powf(delta_L(Label[i][j], Logit[i][j]), (float)2); // (y - y^)^2 계산
		}
	}

	mean += sum / (M.row(Label) * M.col(Label)); // 평균 계산
	loss = mean / 2; // 손실값 계산
	
	return loss;
}

float** Loss::backward(void) {
	Mtrx M;
	float** delta = NULL;
	AllocArr A;
	delta = A.alloc(delta, M.row(Logit), M.col(Logit)); // gradiant 계산을 위해 할당

	for (int i = 0; i < M.row(delta); i++) {
		for (int j = 0; j < M.col(delta); j++) {
			if (delta != NULL) // 컴파일 경고 방지
				delta[i][j] = delta_L(Logit[i][j], Label[i][j]);
		}
	}

	return delta;
}

float Loss::delta_L(float Logit, float Label) {
	return Logit - Label;
}