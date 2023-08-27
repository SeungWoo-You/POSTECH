#include <stdio.h>


int main() {
	int a, w29, m11, b3, s1, c;
	/*a는 입력받는 숫자, w29는 29L, m11은 11L, b3는 3L, s1은 1L, c는 계산용 변수*/ 
	printf("운반가능한 물의 양은? : ");
	scanf("%d", &a);
	w29 = a/29;
	printf("생수통 %d개, ", w29);
	c = a%29;
	/*입력 받은 값을 나눈 나머지를 다시 나누는 반복성을 줄이기 위해 계산용 변수 c 설정*/
	m11 = c/11;
	printf("약수통 %d개, ", m11);
	c = c%11;
	b3 = c/3;
	printf("큰 물통 %d개, ", b3);
	c = c%3;
	s1 = c;
	printf("작은 물통 %d개로 ", s1);
	c = 1000*(w29+m11+b3+s1);
	printf("총 운반 비용은 %d원입니다.", c);
	return 0;
}
