#include <stdio.h>


int main() {
	char a, b, c;
	/*a�� �Է¹��� �빮��, b�� �ҹ���, c�� ���ĺ� ��ȣ*/
	printf("Enter an uppercase character : ");
	scanf("%c", &a);
	b = a+32;
	printf("Lowercase of entered character is %c\n", b);
	c = a-64;
	printf("Position of '%c' in English Alphabets is %d", b, c);
	return 0;
}
