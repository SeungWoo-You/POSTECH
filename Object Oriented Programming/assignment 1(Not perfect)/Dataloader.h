#ifndef DATALOADER_H // 헤더파일 중복 선언 방지
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
	float** label; // row = data_num, col = feature_num. 정확히는 class_num 크기의 col을 가지지만, 행렬 계산을 위해서 data와 같은 크기를 잡아준다.

public:
	Dataloader(); // 생성자
	Dataloader(string mode, int feature_num, int class_num); // 생성자 overloading. mode, feature number, class number 저장
	void read(); // file을 읽고 private의 각 변수에 저장
	string type(); // mode getter
	float** Data(string type); // data or label getter
	void one_hot(int pos, float* label); // label 생성용. 알맞은 클래스 자리에 1을 저장하고 나머지 자리에 0을 저장
};

#endif
