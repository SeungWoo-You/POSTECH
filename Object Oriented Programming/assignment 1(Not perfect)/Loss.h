#ifndef LOSS_H
#define LOSS_H

class Loss {
private:
	float** Logit; // row = data_num, col = feature_num
	float** Label; // row = data_num, col = class_num

public:
	Loss(); // 컴파일러가 계속 변수 초기화 경고 띄워서 생성자 지정
	float forward(float** Logit, float** Label); // Loss function value 계산
	float** backward(void); // gradiant 계산 후 전달
	float delta_L(float Logit, float Label); // gradiant 계산에 필요한 식 함수화
};


#endif

