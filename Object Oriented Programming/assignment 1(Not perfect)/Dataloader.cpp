#include <iostream>
#include <string>
#include<fstream> // ���� �б��
#include "Mlp.h"
using namespace std;
// ��� �ҽ��ڵ� ���Ͽ��� �� �Լ��� ������ ������� ����

Dataloader::Dataloader() {
	file_dir = "";
	mode = "";
	feature_num = 0;
	class_num = 0;
	data_num = 0;
	data = NULL;
	label = NULL;
}

Dataloader::Dataloader(string mode, int feature_num, int class_num) {
	file_dir = "";
	this->mode = mode; // this �����͸� �̿��� private�� ������ ����
	this->feature_num = feature_num;
	this->class_num = class_num;
	data_num = 0;
	data = NULL;
	label = NULL;
}

void Dataloader::read() {
	ifstream fr;

	while (1) { // file directory �Է� ���� �� ���Է� �ޱ� ��
		cout << "Input the file directory(" << mode << "): ";
		cin >> file_dir;

		fr.open(file_dir);
		if (fr.is_open() == 0) { // ������ ���������� ������ �ʾ��� ��(������ �������� ���� ��)
			cout << "File could not be opened!" << endl;
			fr.close(); // �߸��� ���ϰ� ����� file stream ���� ����
			cin.ignore(256, '\n'); // ���� ����
			continue;
		}

		break;
	}

	cout << "Input the number of data(" << mode << "): ";
	cin >> data_num;

	AllocArr A; // ���� �Ҵ�� ��ü A ����. ���� ��� �ҽ��ڵ� ������ A�� ���� �Ҵ�� ��ü�� ���ȴ�.
	data = A.alloc(data, data_num, feature_num);
	if (mode.compare("test") != 0) // train�� validation���� �ִ� label ������ �޾ƿ��� ����
		label = A.alloc(label, data_num, feature_num);
	
	for (int i = 0; i < data_num; i++) { // ���Ͽ� �ִ� data�� label(test ����) �б�
		for (int j = 0; j < feature_num + 1; j++) { // label�� data �ڿ� �پ� �����Ƿ� feature number ��ŭ �ݺ�
			if (j == feature_num) {
				if (mode.compare("test") == 0) // ��, test�� label�� �����Ƿ� feature number ��ŭ�� ���ƾ� �Ѵ�.
					continue;
				else { // label ���� ����
					int pos;
					fr >> pos;
					one_hot(pos, label[i]); // Ŭ���� ��ġ ������ label�� �� row�� ���ڷ� �����Ѵ�
				}
			}

			else // data ���� ����
				fr >> data[i][j];
		}
	}

	return;
}

string Dataloader::type() {
	return mode;
}

float** Dataloader::Data(string type) {
	if (type.compare("data") == 0)
		return data;
	else
		return label;
}

void Dataloader::one_hot(int pos, float* label) {
	Mtrx M;

	for (int i = 0; i < M.size(label); i++) { // ��� ĭ�� 0 ����
		label[i] = (float)0;
	}

	label[pos] = 1; //  ���� Ŭ���� ĭ�� 1 ����
}