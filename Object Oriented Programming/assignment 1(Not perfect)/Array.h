#ifndef MTRX_H
#define MTRX_H

class Mtrx {
private:

public:
	int row(float** arr);
	int col(float** arr);
	int size(float* arr);
	void calc_matrix(float** front, float** rear, float** output);
	void calc_matrix(float* front, float** rear, float* output);
};


#endif
