#include <iostream>
#include <string>
#include <cmath> // exp, powf �Լ� ���
#include <cstdlib> // rand, srand �Լ� ���
#include "Mlp.h"

void Layer::initialize(int in_dimension, int out_dimension) {
	int i = 0, j = 0;

	this->in_dimension = in_dimension;
	this->out_dimension = out_dimension;

	AllocArr A;
	weight = A.alloc(weight, in_dimension, out_dimension);
	for (i = 0; i < in_dimension; i++) {
		for (j = 0; j < out_dimension; j++) {
			weight[i][j] = (rand() / ((float)RAND_MAX)) * 2 - 1; // ����ġ ���� �ʱ�ȭ (-1 ~ 1�� �Ǽ�)
		}
	}

	bias = A.alloc(bias, out_dimension);
	for (i = 0; i < out_dimension; i++) {
		bias[i] = 0; // bias 0���� �ʱ�ȭ
	}

	input = NULL;
	output = NULL;
}

float** Layer::forward(float** data) { // ���̾� �� �� ���
	AllocArr A;
	Mtrx M; // ��� ���� ��ü M ����. ���� ��� �ҽ��ڵ� ������ M�� ��� ���� ��ü�� ���ȴ�.

	input = A.alloc(input, M.row(data), M.col(data)); // input�� ������ �� layer�� ���������� ����Ǿ�� �ϱ⿡ ���� �����Ҵ� �޾Ƽ� ����

	for (int i = 0; i < M.row(input); i++) {
		for (int j = 0; j < M.col(input); j++) {
			input[i][j] = data[i][j];
		}
	}

	A.dealloc(data, M.row(data)); // input�� ���� ������ ����Ǿ������Ƿ� data�� �Ҵ�����.

	float** out = NULL;
	out = M.calc_matrix(input, weight, out); //  input x weight ����� out�� �����Ҵ� �޾� ����. out�� ���� layer�� input���� ���ȴ�(return ��).

	for (int i = 0; i < M.row(out); i++) { // output ���
		for (int j = 0; j < M.col(out); j++) {
			out[i][j] += bias[j];
			out[i][j] = sigmoid(out[i][j]);
		}
	}

	output = A.alloc(out, M.row(out), M.col(out)); // output�� ������ �� layer�� ���������� ����Ǿ�� �ϱ⿡ ���� �����Ҵ� �޾Ƽ� ����

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
			gradiant[i][j] = gradiant[i][j] * delta(output[i][j]); // gradiant ���
		}
	}

	float** delW = NULL, ** input = NULL;
	input = M.transpose(this->input); // private�� input ������ �̿��� ��ġ��� ���
	A.dealloc(this->input, M.row(this->input)); // private�� input ������ �� �̻� ����� ���� �����Ƿ� �Ҵ� ����

	delW = M.calc_matrix(input, gradiant, delW); // gradiant(~~ delta) w ���

	float* delB = NULL, * one_arr = NULL;
	one_arr = A.alloc(one_arr, M.row(gradiant)); // ��� ĭ�� ���� 1�� �迭. gradiant b ��� ��
	
	if (one_arr != NULL) { // ������ ��� ����
		for (int i = 0; i < M.size(one_arr); i++) {
			one_arr[i] = (float)1;
		}
	}

	delB = M.calc_matrix(one_arr, gradiant, delB); // gradiant b ���

	float** return_value = NULL, ** temp = M.transpose(weight); // ����� ���� �ӽ÷� weight�� ��ġ��� ����(�����Ҵ� ����)
	return_value = M.calc_matrix(gradiant, temp, return_value); // ���� layer �� weight & bias update�� �ʿ��� gradiant ���

	update_value(weight, delW, learning_rate); // weight ������Ʈ
	update_value(bias, delB, learning_rate); // bias ������Ʈ

	// �ʿ���� ���� �Ҵ� ����
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