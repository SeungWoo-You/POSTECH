#pragma once

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include "Enum.h"
#include "Object.h"
#include "Map.h"

using namespace std;

Direction KeyInput() // ������ �ٷ� ȭ�� ��ȯ�ȴ�(enter �ʿ� x)
{
	int key;
	key = _getch();
	switch (key)
	{
	case 'w':
		return UP;
	case 'a':
		return LEFT;
	case 'd':
		return RIGHT;
	case 's':
		return DOWN;
	case 'r':
		return RESTART;
	case 'e':
		return EXIT;
	default: // ������ ���� �߻� ���� �� ���� �ð� ������ ���� �ڵ� �߰�
		return CONTINUE;
	}
}

int main()
{
	string filename;

	while (true) {
		cout << "Read Stage File: ";
		cin >> filename;
		if (!Map::ReadMap(filename)) continue; //������ �� �о����� �ٽ� �Է��ϰ� �Ѵ�.
		Map::PrintMap();
		cout << filename << " Load Complete. Press Any Key to Continue...";
		KeyInput();
		while (true) {
			Map::CheckRules(); // ���� ���� ����
			Map::ChangeEntity(); // ������Ʈ �ٲٱ�
			Map::LoseCheck(); // lose�� ���� ���� Ȯ��
			Map::PrintMap();  // �ٲ� �� ���
			Map::PrintRules(); // �ʿ��ִ� ���� ���
			if (Map::CantMoveCheck()) { // YOU ��ü�� ���� �� �й�
				cout << "YOU does not exist. You Lose" << endl;
				break;
			}
			if (Map::WinCheck()) { // �¸� ���� ������ �¸�
				cout << "YOU WIN!" << endl;
				break;
			}
			cout << "Key Input: w=up, s=down, a=left, d=right, r=restart, e=exit" << endl;
			Direction dir = KeyInput(); // Ű �Է��� �޴´�.
			if (dir == CONTINUE)	// ������ Ű ���� �Է��� ������ �������� �ʴ´�
				continue;
			
			if (dir == RESTART) {
				Map::RemoveAll();	// �Ҵ���� ��� �޸� ����
				Map::ReadMap(filename); continue;
			}
			if (dir == EXIT) {
				Map::RemoveAll();	// �Ҵ���� ��� �޸� ����
				system("cls");
				break;
			}

			Map::YouMove(dir);
		}
	}

	return 0;
}