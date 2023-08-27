#include <stdio.h>

char lowerToUpper(char slet);
int getSequenceNum(char blet);
char encrypt(char blet, int num);

int main() {
	char slet, blet, enc;
	int num, seq;

	printf("Enter a character in Lower case: ");
	scanf_s("%c", &slet, sizeof slet);
	printf("Enter encoding number(1~9): ");
	scanf_s("%d", &num);

	blet = lowerToUpper(slet);
	seq = getSequenceNum(blet);
	enc = encrypt(blet, num);

	printf("Uppercase of Entered character: %c\n", blet);
	printf("Sequence Number of entered character: %d\n", seq);
	printf("Encrypted output: %c\n", enc);

	return 0;
}

char lowerToUpper(char slet) {
	return slet - 97 + 65;
}

int getSequenceNum(char blet) {
	return (int)(blet - 64);
}

char encrypt(char blet, int num) {
	return blet - num;
}