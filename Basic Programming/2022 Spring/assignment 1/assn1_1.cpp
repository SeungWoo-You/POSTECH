#include <stdio.h>

char ruby_encryptor(char input, char key);
char Small_to_Capital(char small);

int main() {
	char initial_1, initial_2, initial_3;
	char keyword_1, keyword_2, keyword_3;
	char ruby_1, ruby_2, ruby_3;

	printf("Enter your initials to encrypt (length : 3) :");		// 1. �̴ϼ� ���ĺ� �Է¹ޱ�
	scanf("%c %c %c", &initial_1, &initial_2, &initial_3);
	while (getchar() != '\n');

	printf("Enter your keyword (length : 3) :");			// 2. �ҹ��� Ű���� �Է� �ޱ�
	scanf("%c %c %c", &keyword_1, &keyword_2, &keyword_3);
	while (getchar() != '\n');

	ruby_1 = ruby_encryptor(initial_1, keyword_1);			// 3. �̴ϼ� ��ȣȭ
	ruby_2 = ruby_encryptor(initial_2, keyword_2);
	ruby_3 = ruby_encryptor(initial_3, keyword_3);

	printf("%c%c%c", Small_to_Capital(ruby_1), Small_to_Capital(ruby_2), Small_to_Capital(ruby_3));		// 4. ��ȯ �� ���


	return 0;
}

char ruby_encryptor(char input, char key) {			// ��ü ����: ASCII ���� -> ���ĺ� ���� -> ��ȣȭ ���� -> ASCII ����
	char encryption_value = 0, result = 0;

	encryption_value = key - 'a';
	result = (input - 'a' + 1 + encryption_value) % 26 + 'a' - 1;

	return result;
}

char Small_to_Capital(char small) {
	return small - 32;
}