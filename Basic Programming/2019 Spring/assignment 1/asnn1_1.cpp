#include <stdio.h>
#include <math.h>

float CalcS(int won, float per, int year);
float CalcC(int won, float per, int year);
float PertoProp(float per);

int main() {
	int won, year;
	float per, Ssum, Csum;

	printf("Enter the principal(won), interest(%%) and period(year) with space: ");
	scanf_s("%d%f%d", &won, &per, &year);

	per = PertoProp(per);
	Ssum = CalcS(won, per, year);
	Csum = CalcC(won, per, year);

	printf("Simple Interest: %.0fwon\n", Ssum);
	printf("Compound Interest: %.0fwon\n", Csum);

	return 0;
}

float CalcS(int won, float per, int year) {
	float sum;
	return sum = won * (1 + (per * year));
}

float CalcC(int won, float per, int year) {
	float sum;
	return sum = won * pow(1 + per, year);
}

float PertoProp(float per) {
	float prop;
	return prop = per / 100;
}