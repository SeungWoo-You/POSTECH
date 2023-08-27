#include <iostream>
#include <string>
#include "Mlp.h"

Loss::Loss() {
	Logit = NULL;
	Label = NULL;
}

float Loss::forward(float** Logit, float** Label) {
	this->Logit = Logit; // �� ���� private ������ ����
	this->Label = Label;

	Mtrx M;
	float loss = 0, sum = 0, mean = 0;

	for (int i = 0; i < M.row(Label); i++) {
		for (int j = 0; j < M.col(Label); j++) {
			sum += powf(delta_L(Label[i][j], Logit[i][j]), (float)2); // (y - y^)^2 ���
		}
	}

	mean += sum / (M.row(Label) * M.col(Label)); // ��� ���
	loss = mean / 2; // �սǰ� ���
	
	return loss;
}

float** Loss::backward(void) {
	Mtrx M;
	float** delta = NULL;
	AllocArr A;
	delta = A.alloc(delta, M.row(Logit), M.col(Logit)); // gradiant ����� ���� �Ҵ�

	for (int i = 0; i < M.row(delta); i++) {
		for (int j = 0; j < M.col(delta); j++) {
			if (delta != NULL) // ������ ��� ����
				delta[i][j] = delta_L(Logit[i][j], Label[i][j]);
		}
	}

	return delta;
}

float Loss::delta_L(float Logit, float Label) {
	return Logit - Label;
}