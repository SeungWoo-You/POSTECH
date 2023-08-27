#include <stdio.h>
#include <math.h>

float calc_dist(int x1, int y1, int x2, int y2);
float calc_area(int x1, int y1, int x2, int y2, int x3, int y3);

int main() {
	int x1, x2, x3;
	int y1, y2, y3;
	float perimeter, area;

	printf("Enter P1(x1, y1) : ");			// 1. 좌표 입력받기
	scanf("%d%d", &x1, &y1);
	printf("Enter P2(x2, y2) : ");
	scanf("%d%d", &x2, &y2);
	printf("Enter P3(x3, y3) : ");
	scanf("%d%d", &x3, &y3);

	perimeter = calc_dist(x1, y1, x2, y2) + calc_dist(x2, y2, x3, y3) + calc_dist(x3, y3, x1, y1);			// 2-1. 둘레 계산
	area = calc_area(x1, y1, x2, y2, x3, y3);			// 2-2. 넓이 계산

	printf("The perimeter of triangle is: %.2f\n", perimeter);			// 3. 출력
	printf("The area of triangle is: %.2f", area);

	
	return 0;
}

float calc_dist(int x1, int y1, int x2, int y2) {
	float result;

	result = sqrt(pow(x2 - (double)x1, 2) + pow(y2 - (double)y1, 2));

	return result;
}

float calc_area(int x1, int y1, int x2, int y2, int x3, int y3) {
	float result;

	result = fabs((x1 * (double)y2 + x2 * (double)y3 + x3 * (double)y1) - (x2 * (double)y1 + x3 * (double)y2 + x1 * (double)y3)) / 2;

	return result;
}
