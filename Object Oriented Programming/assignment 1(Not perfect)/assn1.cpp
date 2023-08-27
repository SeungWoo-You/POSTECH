#include <iostream>
#include <string>
#include "Mlp.h"
#include <ctime> // rand �Լ� seed ��
using namespace std;

int main() {
	Mlp machine_learning_program; // ���α׷� ���࿡ ����ϴ� ��ü(���� mlp)
	string test_output_dir = ""; // test output directory
	float learning_rate = 0;
	int epochs = 0, feature_num = 0, class_num = 0;

	srand((unsigned int)time(NULL)); // �ʱ� weight�� ���� ������ �ֱ� ���� ����

	cout << "Input the number of feature: ";
	cin >> feature_num;
	cout << "Input the number of class: ";
	cin >> class_num;

	Dataloader train_dataset("train", feature_num, class_num), val_dataset("val", feature_num, class_num), test_dataset("test", feature_num, class_num); // �� ��ü�� ������ ȣ��
	machine_learning_program.read_dataset(train_dataset); // train dataset�� ����(data, label)�� ���Ͽ��� �о�� mlp ��ü�� ����
	machine_learning_program.read_dataset(val_dataset); // validation dataset�� ����(data, label)�� ���Ͽ��� �о�� mlp ��ü�� ����
	test_dataset.read(); // test dataset�� ������ ���Ͽ��� �о�� test_dataset ��ü�� ����

	machine_learning_program.add_layer(); // hidden layer �� output layer ����
	
	cout << "Input the learning rate: ";
	cin >> learning_rate;
	machine_learning_program.input_data(learning_rate); // �н��� mlp ��ü�� ����

	cout << "Input the number of epoch: ";
	cin >> epochs;
	machine_learning_program.input_data(epochs); // �Ʒ� Ƚ�� mlp ��ü�� ����

	cout << "Input the directory to save the test output: ";
	cin >> test_output_dir;

	machine_learning_program.visualize_layer(); // layers(hidden, output)�� ���� ���

	for (int i = 1; i <= epochs; i++) { // �ӽŷ��� ����
		cout << "epoch: " << i << " ";
		machine_learning_program.train(); // training �� ���(loss, accuracy) ���
	}

	machine_learning_program.validation(); // training ����

	machine_learning_program.predict(test_dataset, test_output_dir); // test�� dataset�� �̿��� �ӽŷ��� test �� ��� ���

	return 0;
}