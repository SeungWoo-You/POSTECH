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
	void initialize(int in_dimension, int out_dimension); // setter. layer ���� �����Ҵ� ���� �� �⺻ �����ڰ� �־�� �ϱ⿡ ������ ��� setter�� �̿�
	float** forward(float** data); // ���� ���̾��� input�� ���� ���� ���̾ ������ output���
	float** backward(float** gradiant, float learning_rate); // ����ġ�� bias ��� �� ������Ʈ �� gradiant�� ���� ���̾ ����
	float sigmoid(float x); // sigmoid ��� ��
	float delta(float x); // gradiant ��꿡 �ʿ��� ��(a)(1 ?��(a)) ��� ��(���� sigmoid)
	void update_value(float** goal, float** delta, float learning_rate); // weight update
	void update_value(float* goal, float* delta, float learning_rate); // overloading. bias update
	float** Input(void); // input getter
	float** Weight(void); // weight getter
	float** Output(void); // output getter
	float* Bias(void); // bias getter
};

#endif
