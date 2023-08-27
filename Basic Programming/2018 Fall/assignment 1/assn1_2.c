#include <stdio.h>


int main() {
	char a, b, c;
	/*a는 입력받을 대문자, b는 소문자, c는 알파벳 번호*/
	printf("Enter an uppercase character : ");
	scanf("%c", &a);
	b = a+32;
	printf("Lowercase of entered character is %c\n", b);
	c = a-64;
	printf("Position of '%c' in English Alphabets is %d", b, c);
	return 0;
}
