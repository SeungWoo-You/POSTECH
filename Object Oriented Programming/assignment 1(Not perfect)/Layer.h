#ifndef LAYER_H
#define LAYER_H

class Layer {
private:
	int in_dimension;
	int out_dimension;
	float** weight; // row = in_dimension, col = out_dimension
	float* bias; // size = out_dimension
	float** input; // row = data_num, col = in_dimension
	float** output; //row = data num, col = out_dimenstion

public:
	void initialize(int in_dimension, int out_dimension); // setter. layer 수를 동적할당 받을 때 기본 생성자가 있어야 하기에 생성자 대신 setter를 이용
	float** forward(float** data); // 이전 레이어의 input을 통해 다음 레이어에 전달할 output계산
	float** backward(float** gradiant, float learning_rate); // 가중치와 bias 계산 및 업데이트 후 gradiant를 이전 레이어에 전달
	float sigmoid(float x); // sigmoid 계산 용
	float delta(float x); // gradiant 계산에 필요한 ＠(a)(1 ?＠(a)) 계산 용(＠는 sigmoid)
	void update_value(float** goal, float** delta, float learning_rate); // weight update
	void update_value(float* goal, float* delta, float learning_rate); // overloading. bias update
	float** Input(void); // input getter
	float** Weight(void); // weight getter
	float** Output(void); // output getter
	float* Bias(void); // bias getter
};

#endif
