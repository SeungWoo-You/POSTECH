#ifndef MLP_H
#define MLP_H
#include "AllocArr.h" // 컴파일 순서 고려해서 include
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
	Mlp(); // 컴파일러가 계속 변수 초기화 경고 띄워서 생성자 지정
	void read_dataset(Dataloader dataset); // 일부 private 변수에 값 저장(setter 역할)
	void add_layer(void); // hidden layer 및 output layer 생성
	void visualize_layer(void); // layers(hidden, output)의 정보 출력
	void train(void); // training 후 결과(loss, accuracy) 출력
	void validation(void); // training 검증
	void predict(Dataloader test_dataset, std::string result_dir); // test용 dataset을 이용해 머신러닝 test 후 결과 출력
	void line(int n); // 구분선 출력
	float accuracy(float** output, float** label); // 정확도 계산
	int find_class(float* data); // 클래스 찾기
	void input_data(float learning_rate); // learnning_rate setter
	void input_data(int epochs); // overloading. epochs setter
	~Mlp(); // 동적 할당 해제를 위한 소멸자
};



#endif