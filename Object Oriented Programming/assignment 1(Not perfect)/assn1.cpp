#include <iostream>
#include <string>
#include "Mlp.h"
#include <ctime> // rand 함수 seed 용
using namespace std;

int main() {
	Mlp machine_learning_program; // 프로그램 진행에 사용하는 객체(이하 mlp)
	string test_output_dir = ""; // test output directory
	float learning_rate = 0;
	int epochs = 0, feature_num = 0, class_num = 0;

	srand((unsigned int)time(NULL)); // 초기 weight를 랜덤 값으로 주기 위한 설정

	cout << "Input the number of feature: ";
	cin >> feature_num;
	cout << "Input the number of class: ";
	cin >> class_num;

	Dataloader train_dataset("train", feature_num, class_num), val_dataset("val", feature_num, class_num), test_dataset("test", feature_num, class_num); // 각 객체들 생성자 호출
	machine_learning_program.read_dataset(train_dataset); // train dataset의 정보(data, label)를 파일에서 읽어와 mlp 객체에 저장
	machine_learning_program.read_dataset(val_dataset); // validation dataset의 정보(data, label)를 파일에서 읽어와 mlp 객체에 저장
	test_dataset.read(); // test dataset의 정보를 파일에서 읽어와 test_dataset 객체에 저장

	machine_learning_program.add_layer(); // hidden layer 및 output layer 생성
	
	cout << "Input the learning rate: ";
	cin >> learning_rate;
	machine_learning_program.input_data(learning_rate); // 학습률 mlp 객체에 저장

	cout << "Input the number of epoch: ";
	cin >> epochs;
	machine_learning_program.input_data(epochs); // 훈련 횟수 mlp 객체에 저장

	cout << "Input the directory to save the test output: ";
	cin >> test_output_dir;

	machine_learning_program.visualize_layer(); // layers(hidden, output)의 정보 출력

	for (int i = 1; i <= epochs; i++) { // 머신러닝 시작
		cout << "epoch: " << i << " ";
		machine_learning_program.train(); // training 후 결과(loss, accuracy) 출력
	}

	machine_learning_program.validation(); // training 검증

	machine_learning_program.predict(test_dataset, test_output_dir); // test용 dataset을 이용해 머신러닝 test 후 결과 출력

	return 0;
}