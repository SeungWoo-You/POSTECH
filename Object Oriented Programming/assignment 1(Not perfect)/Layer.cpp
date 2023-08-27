#include <iostream>
#include <string>
#include <cmath> // exp, powf 함수 사용
#include <cstdlib> // rand, srand 함수 사용
#include "Mlp.h"

void Layer::initialize(int in_dimension, int out_dimension) {
	int i = 0, j = 0;

	this->in_dimension = in_dimension;
	this->out_dimension = out_dimension;

	AllocArr A;
	weight = A.alloc(weight, in_dimension, out_dimension);
	for (i = 0; i < in_dimension; i++) {
		for (j = 0; j < out_dimension; j++) {
			weight[i][j] = (rand() / ((float)RAND_MAX)) * 2 - 1; // 가중치 랜덤 초기화 (-1 ~ 1의 실수)
		}
	}

	bias = A.alloc(bias, out_dimension);
	for (i = 0; i < out_dimension; i++) {
		bias[i] = 0; // bias 0으로 초기화
	}

	input = NULL;
	output = NULL;
}

float** Layer::forward(float** data) { // 레이어 한 층 계산
	AllocArr A;
	Mtrx M; // 행렬 계산용 객체 M 생성. 이하 모든 소스코드 파일의 M은 행렬 계산용 객체로 사용된다.

	input = A.alloc(input, M.row(data), M.col(data)); // input의 정보는 각 layer에 독립적으로 저장되어야 하기에 따로 동적할당 받아서 저장

	for (int i = 0; i < M.row(input); i++) {
		for (int j = 0; j < M.col(input); j++) {
			input[i][j] = data[i][j];
		}
	}

	A.dealloc(data, M.row(data)); // input에 같은 정보가 저장되어있으므로 data는 할당해제.

	float** out = NULL;
	out = M.calc_matrix(input, weight, out); //  input x weight 결과를 out에 동적할당 받아 저장. out은 다음 layer의 input으로 사용된다(return 용).

	for (int i = 0; i < M.row(out); i++) { // output 계산
		for (int j = 0; j < M.col(out); j++) {
			out[i][j] += bias[j];
			out[i][j] = sigmoid(out[i][j]);
		}
	}

	output = A.alloc(out, M.row(out), M.col(out)); // output의 정보는 각 layer에 독립적으로 저장되어야 하기에 따로 동적할당 받아서 저장

	for (int i = 0; i < M.row(out); i++) {
		for (int j = 0; j < M.col(out); j++) {
			output[i][j] = out[i][j];
		}
	}

	return out;
}

float** Layer::backward(float** gradiant, float learning_rate) {
	Mtrx M;
	AllocArr A;
	
	for (int i = 0; i < M.row(gradiant); i++) {
		for (int j = 0; j < M.col(gradiant); j++) {
			gradiant[i][j] = gradiant[i][j] * delta(output[i][j]); // gradiant 계산
		}
	}

	float** delW = NULL, ** input = NULL;
	input = M.transpose(this->input); // private의 input 정보를 이용해 전치행렬 계산
	A.dealloc(this->input, M.row(this->input)); // private의 input 정보는 더 이상 사용할 일이 없으므로 할당 해제

	delW = M.calc_matrix(input, gradiant, delW); // gradiant(~~ delta) w 계산

	float* delB = NULL, * one_arr = NULL;
	one_arr = A.alloc(one_arr, M.row(gradiant)); // 모든 칸의 값이 1인 배열. gradiant b 계산 용
	
	if (one_arr != NULL) { // 컴파일 경고 방지
		for (int i = 0; i < M.size(one_arr); i++) {
			one_arr[i] = (float)1;
		}
	}

	delB = M.calc_matrix(one_arr, gradiant, delB); // gradiant b 계산

	float** return_value = NULL, ** temp = M.transpose(weight); // 계산을 위해 임시로 weight의 전치행렬 설정(동적할당 받음)
	return_value = M.calc_matrix(gradiant, temp, return_value); // 이전 layer 의 weight & bias update에 필요한 gradiant 계산

	update_value(weight, delW, learning_rate); // weight 업데이트
	update_value(bias, delB, learning_rate); // bias 업데이트

	// 필요없는 정보 할당 해제
	A.dealloc(temp, M.row(temp));
	A.dealloc(one_arr);
	A.dealloc(input, M.row(input));
	A.dealloc(delW, M.row(delW));
	A.dealloc(delB);
	A.dealloc(gradiant, M.row(gradiant));

	return return_value;
}

float Layer::sigmoid(float x) {
	return 1 / (1 + exp(-x));
}

float Layer::delta(float x) {
	return x * (1 - x);
}

void Layer::update_value(float** goal, float** delta, float learning_rate) {
	Mtrx M;

	for (int i = 0; i < M.row(goal); i++) {
		for (int j = 0; j < M.col(goal); j++) {
			goal[i][j] = goal[i][j] - learning_rate * delta[i][j];
		}
	}
}

void Layer::update_value(float* goal, float* delta, float learning_rate) {
	Mtrx M;

	for (int i = 0; i < M.size(goal); i++) {
		goal[i] = goal[i] - learning_rate * delta[i];
	}
}

float** Layer::Input(void) {
	return input;
}

float** Layer::Weight(void) {
	return weight;
}

float** Layer::Output(void) {
	return output;
}

float* Layer::Bias(void) {
	return bias;
}