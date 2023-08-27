#ifndef LOSS_H
#define LOSS_H

class Loss {
private:
	float** Logit; // row = data_num, col = feature_num
	float** Label; // row = data_num, col = class_num

public:
	Loss(); // �����Ϸ��� ��� ���� �ʱ�ȭ ��� ����� ������ ����
	float forward(float** Logit, float** Label); // Loss function value ���
	float** backward(void); // gradiant ��� �� ����
	float delta_L(float Logit, float Label); // gradiant ��꿡 �ʿ��� �� �Լ�ȭ
};


#endif

