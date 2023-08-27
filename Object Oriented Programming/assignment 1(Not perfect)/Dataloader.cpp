#include <iostream>
#include <string>
#include<fstream> // 파일 읽기용
#include "Mlp.h"
using namespace std;
// 모든 소스코드 파일에서 각 함수의 역할은 헤더파일 참고

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
	this->mode = mode; // this 포인터를 이용해 private의 변수에 접근
	this->feature_num = feature_num;
	this->class_num = class_num;
	data_num = 0;
	data = NULL;
	label = NULL;
}

void Dataloader::read() {
	ifstream fr;

	while (1) { // file directory 입력 오류 시 재입력 받기 용
		cout << "Input the file directory(" << mode << "): ";
		cin >> file_dir;

		fr.open(file_dir);
		if (fr.is_open() == 0) { // 파일이 정상적으로 열리지 않았을 때(파일이 존재하지 않을 때)
			cout << "File could not be opened!" << endl;
			fr.close(); // 잘못된 파일과 연결된 file stream 연결 해제
			cin.ignore(256, '\n'); // 버퍼 비우기
			continue;
		}

		break;
	}

	cout << "Input the number of data(" << mode << "): ";
	cin >> data_num;

	AllocArr A; // 동적 할당용 객체 A 생성. 이하 모든 소스코드 파일의 A는 동적 할당용 객체로 사용된다.
	data = A.alloc(data, data_num, feature_num);
	if (mode.compare("test") != 0) // train과 validation에만 있는 label 정보를 받아오기 위해
		label = A.alloc(label, data_num, feature_num);
	
	for (int i = 0; i < data_num; i++) { // 파일에 있는 data와 label(test 제외) 읽기
		for (int j = 0; j < feature_num + 1; j++) { // label이 data 뒤에 붙어 있으므로 feature number 만큼 반복
			if (j == feature_num) {
				if (mode.compare("test") == 0) // 단, test는 label이 없으므로 feature number 만큼만 돌아야 한다.
					continue;
				else { // label 정보 저장
					int pos;
					fr >> pos;
					one_hot(pos, label[i]); // 클래스 위치 정보와 label의 각 row를 인자로 전달한다
				}
			}

			else // data 정보 저장
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

	for (int i = 0; i < M.size(label); i++) { // 모든 칸에 0 저장
		label[i] = (float)0;
	}

	label[pos] = 1; //  정답 클래스 칸만 1 저장
}