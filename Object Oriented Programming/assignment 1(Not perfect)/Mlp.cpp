#include <iostream>
#include <string>
#include <sstream>
#include<fstream>
#include "Mlp.h"
using namespace std;

Mlp::Mlp() {
	layers = nullptr;
	train_data = NULL;
	train_label = NULL;
	validation_data = NULL;
	validation_label = NULL;
	epochs = 0;
	learning_rate = 0;
	layer_num = 0;
}

void Mlp::read_dataset(Dataloader dataset) {
	dataset.read(); // data read

	string mode = "train";
	if (mode.compare(dataset.type()) == 0) { // mode에 맞춰서 각 private 변수에 값 저장
		train_dataset = dataset;
		train_data = dataset.Data("data");
		train_label = dataset.Data("label");
		return;
	}
	
	mode = "val";
	if (mode.compare(dataset.type()) == 0) {
		validation_dataset = dataset;
		validation_data = dataset.Data("data");
		validation_label = dataset.Data("label");
		return;
	}
}

void Mlp::add_layer(void) {
	int hidden_layer_num = 0, i = 0;

	cout << "Input the number of hidden layer: ";
	cin >> hidden_layer_num;
	layer_num = hidden_layer_num + 1; // output layer가 있으므로 전체 layer 수는 hidden layer number + 1

	layers = new Layer[layer_num]; // 객체 배열 설정

	string str;
	stringstream hidden_layer_neurons; // 원하는 정보(레이어 뉴런 수)를 공백 기준으로 쉽게 빼내기 위해 stringstream 이용
	cout << "Input the number of neuron of each hidden layer: ";
	cin.ignore(256, '\n'); // 버퍼 초기화
	getline(cin, str); // 라인 전체 받기

	Mtrx M;
	int in_dimension = M.col(train_data);
	int out_dimension = 0;

	hidden_layer_neurons.str(str);
	for (i = 0; i < layer_num; i++) {
		hidden_layer_neurons >> str;
		out_dimension = stoi(str); // int형으로 저장해야 한다

		if (out_dimension == 0) { // stringstream이 똑바로 되지 않은 경우
			cout << "Dimension error!" << endl;
			exit(1); // 강제 종료
		}
		
		if (i == hidden_layer_num) // 마지막 output layer 설정
			layers[i].initialize(in_dimension, M.col(train_data));

		else // hidden layer 설정
			layers[i].initialize(in_dimension, out_dimension);

		in_dimension = out_dimension;
	}

	

}

void Mlp::visualize_layer(void) {
	Mtrx M;

	line(40);

	for (int i = 0; i < layer_num; i++)
		cout << "dense layer " << i << " in_dimension: " << M.row(layers[i].Weight()) << " out_dimension: " << M.col(layers[i].Weight()) << endl;

	line(40);
}

void Mlp::train(void) {
	AllocArr A;
	Mtrx M;
	float** data_output = NULL, ** use_train_data = NULL;
	int i = 0, j = 0;

	use_train_data = A.alloc(use_train_data, M.row(train_data), M.col(train_data)); // train_data를 계속 써야 하므로 수정할 수 있게 use_train_data 할당

	for (i = 0; i < M.row(use_train_data); i++) { // 데이터 복사
		for (j = 0; j < M.col(use_train_data); j++) {
			use_train_data[i][j] = train_data[i][j];
		}
	}
	
	for (i = 0; i < layer_num; i++) { // 방금 할당받은 공간은 Layer::forward 내부에서 해제되고, 다시 할당된 공간이 use_train_data에 저장된다 -> 반복 -> 마지막 한 번 할당 해제 필요
		use_train_data = layers[i].forward(use_train_data);
	}

	data_output = A.alloc(data_output, M.row(use_train_data), M.col(use_train_data));
	for (i = 0; i < M.row(data_output); i++) { // label과의 비교용 set 제작
		float max = use_train_data[i][0];
		int max_pos = 0;

		if (data_output != NULL) {
			data_output[i][max_pos] = 1;

			for (j = 1; j < M.col(data_output); j++) { // row의 값 중에서 가장 큰 값을 1, 나머지를 0으로 저장
				if (max < use_train_data[i][j]) {
					data_output[i][max_pos] = 0;
					max = use_train_data[i][j];
					max_pos = j;
					data_output[i][max_pos] = 1;
				}

				else
					data_output[i][j] = 0;
			}
		}
	}

	float loss; // 손실함수 값 출력용
	loss = this->loss.forward(use_train_data, train_label);
	
	float** gradiant; // gradiant 계산용
	gradiant = this->loss.backward();

	for (i = layer_num - 1; i >= 0; i--) { // Loss::backward에서 할당된 공간은 Layer::backward에서 해제되고, 다시 할당된 공간이 gradiant에 저장된다 -> 반복 -> 마지막 한 번 할당 해제 필요
		gradiant = layers[i].backward(gradiant, learning_rate);
	}

	cout << "train_loss: " << loss << " train accuracy: " << accuracy(data_output, train_label) << endl;

	// 안쓰는 공간 할당해제
	A.dealloc(use_train_data, M.row(use_train_data));
	A.dealloc(gradiant, M.row(gradiant));
	A.dealloc(data_output, M.row(data_output));

	for (i = 0; i < layer_num; i++) {
		A.dealloc(layers[i].Output(), M.row(layers[i].Output()));
	}
}

void Mlp::validation(void) {
	AllocArr A;
	Mtrx M;
	float** data_output = NULL, ** use_val_data = NULL;
	int i = 0, j = 0;

	use_val_data = A.alloc(use_val_data, M.row(validation_data), M.col(validation_data)); // train에서 설정한 use_train_data와 같은 용도

	for (i = 0; i < M.row(use_val_data); i++) {
		for (j = 0; j < M.col(use_val_data); j++) {
			use_val_data[i][j] = validation_data[i][j];
		}
	}



	for (i = 0; i < layer_num; i++) { // 이것도 역시 마지막 한 번 할당 해제 필요
		use_val_data = layers[i].forward(use_val_data);
	}

	data_output = A.alloc(data_output, M.row(use_val_data), M.col(use_val_data));
	for (i = 0; i < M.row(data_output); i++) { // label과의 비교용 set 제작
		float max = use_val_data[i][0];
		int max_pos = 0;
		if (data_output != NULL) {
			data_output[i][max_pos] = 1;

			for (j = 1; j < M.col(data_output); j++) {
				if (max < use_val_data[i][j]) {
					data_output[i][max_pos] = 0;
					max = use_val_data[i][j];
					max_pos = j;
					data_output[i][max_pos] = 1;
				}

				else {
					data_output[i][j] = 0;
				}
			}
		}
	}

	float loss; // 손실함수 값 출력용
	loss = this->loss.forward(use_val_data, validation_label);

	cout << "validation_loss: " << loss << " validation accuracy: " << accuracy(data_output, validation_label) << endl;
	
	// 안쓰는 공간 할당해제
	A.dealloc(data_output, M.row(data_output));
	A.dealloc(use_val_data, M.row(use_val_data));
}

void Mlp::predict(Dataloader test_dataset, string result_dir) {
	float** test_data = test_dataset.Data("data"); //test_dataset의 data 정보를 쓰기 위한 변수 지정
	AllocArr A;
	Mtrx M;

	for (int i = 0; i < layer_num; i++) { // train test
		test_data = layers[i].forward(test_data);
	}

	ofstream fr(result_dir); // 파일 출력
	if (fr.is_open()) { // 파일이 정상적으로 열린 경우
		for (int i = 0; i < M.row(test_data); i++) { // 최대값을 가지는 position이 class value 이므로 최대값을 가지는 position 찾기
			float max = test_data[i][0];
			int max_pos = 0;

			for (int j = 1; j < M.col(test_data); j++) { // 하나의 row 검사해서 position 찾기
				if (max < test_data[i][j]) {
					max = test_data[i][j];
					max_pos = j;
				}

				else
					continue;
			}

			fr << max_pos << endl; // 해당 postion 파일에 출력
		}
	}

	// 다 쓴 공간 할당 해제
	A.dealloc(test_data, M.row(test_data));
	for (int i = 0; i < layer_num; i++) {
		A.dealloc(layers[i].Input(), M.row(layers[i].Input()));
		A.dealloc(layers[i].Output(), M.row(layers[i].Output()));
	}
}

void Mlp::line(int n) {
	for (int i = 0; i < n; i++) {
		cout << "=";
	}
	cout << endl;
}

float Mlp::accuracy(float** output, float** label) {
	int total = 1; int correct = 0, pos_o = 0, pos_l = 0;
	Mtrx M;

	total = M.row(label);
	for (int i = 0; i < M.row(label); i++) {
		if (find_class(output[i]) == find_class(label[i]))
			correct++;
	}

	return ((float)correct) / total;
}

int Mlp::find_class(float* data) { // data는 이차원 배열의 row 한 개
	Mtrx M;
	for (int i = 0; i < M.size(data); i++) {
		if (data[i] == 1)
			return i;
	}

	return 0;
}

void Mlp::input_data(float learning_rate) {
	this->learning_rate = learning_rate;
}

void Mlp::input_data(int epochs) {
	this->epochs = epochs;
}

Mlp::~Mlp() { // 프로그램이 종료될 때 해제해야 할 공간들을 소멸자를 통해 할당 해제
	AllocArr A;
	Mtrx M;
	int i = 0, j = 0;
	//system("pause");
	A.dealloc(train_data, M.row(train_data));
	A.dealloc(train_label, M.row(train_label));
	
	A.dealloc(validation_data, M.row(validation_data));
	A.dealloc(validation_label, M.row(validation_label));

	for (i = 0; i < layer_num; i++) {
		A.dealloc(layers[i].Bias());
	}

	delete[] layers;
}