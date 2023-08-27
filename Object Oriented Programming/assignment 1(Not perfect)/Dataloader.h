#ifndef DATALOADER_H // ������� �ߺ� ���� ����
#define DATALOADER_H
using namespace std;

class Dataloader {
private:
	string file_dir;
	string mode;
	int feature_num;
	int class_num;
	int data_num;
	float** data; // row = data_num, col = feature_num
	float** label; // row = data_num, col = feature_num. ��Ȯ���� class_num ũ���� col�� ��������, ��� ����� ���ؼ� data�� ���� ũ�⸦ ����ش�.

public:
	Dataloader(); // ������
	Dataloader(string mode, int feature_num, int class_num); // ������ overloading. mode, feature number, class number ����
	void read(); // file�� �а� private�� �� ������ ����
	string type(); // mode getter
	float** Data(string type); // data or label getter
	void one_hot(int pos, float* label); // label ������. �˸��� Ŭ���� �ڸ��� 1�� �����ϰ� ������ �ڸ��� 0�� ����
};

#endif
