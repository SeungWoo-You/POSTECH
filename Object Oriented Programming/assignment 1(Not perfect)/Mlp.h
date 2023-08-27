#ifndef MLP_H
#define MLP_H
#include "AllocArr.h" // ������ ���� ����ؼ� include
#include "Mtrx.h"
#include "Dataloader.h"
#include "Layer.h"
#include "Loss.h"

class Mlp {
private:
	Layer* layers;
	Loss loss;
	Dataloader train_dataset;
	float** train_data;
	float** train_label;
	Dataloader validation_dataset;
	float** validation_data;
	float** validation_label;
	int epochs;
	float learning_rate;
	int layer_num;

public:
	Mlp(); // �����Ϸ��� ��� ���� �ʱ�ȭ ��� ����� ������ ����
	void read_dataset(Dataloader dataset); // �Ϻ� private ������ �� ����(setter ����)
	void add_layer(void); // hidden layer �� output layer ����
	void visualize_layer(void); // layers(hidden, output)�� ���� ���
	void train(void); // training �� ���(loss, accuracy) ���
	void validation(void); // training ����
	void predict(Dataloader test_dataset, std::string result_dir); // test�� dataset�� �̿��� �ӽŷ��� test �� ��� ���
	void line(int n); // ���м� ���
	float accuracy(float** output, float** label); // ��Ȯ�� ���
	int find_class(float* data); // Ŭ���� ã��
	void input_data(float learning_rate); // learnning_rate setter
	void input_data(int epochs); // overloading. epochs setter
	~Mlp(); // ���� �Ҵ� ������ ���� �Ҹ���
};



#endif