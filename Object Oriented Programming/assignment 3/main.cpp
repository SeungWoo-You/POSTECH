#pragma once

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include "Enum.h"
#include "Object.h"
#include "Map.h"

using namespace std;

Direction KeyInput() // 누르면 바로 화면 전환된다(enter 필요 x)
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
	default: // 잠재적 오류 발생 방지 및 실행 시간 단축을 위해 코드 추가
		return CONTINUE;
	}
}

int main()
{
	string filename;

	while (true) {
		cout << "Read Stage File: ";
		cin >> filename;
		if (!Map::ReadMap(filename)) continue; //파일을 못 읽었으면 다시 입력하게 한다.
		Map::PrintMap();
		cout << filename << " Load Complete. Press Any Key to Continue...";
		KeyInput();
		while (true) {
			Map::CheckRules(); // 문장 존재 여부
			Map::ChangeEntity(); // 오브젝트 바꾸기
			Map::LoseCheck(); // lose로 인한 삭제 확인
			Map::PrintMap();  // 바뀐 맵 출력
			Map::PrintRules(); // 맵에있는 문장 출력
			if (Map::CantMoveCheck()) { // YOU 개체가 없을 시 패배
				cout << "YOU does not exist. You Lose" << endl;
				break;
			}
			if (Map::WinCheck()) { // 승리 조건 만족시 승리
				cout << "YOU WIN!" << endl;
				break;
			}
			cout << "Key Input: w=up, s=down, a=left, d=right, r=restart, e=exit" << endl;
			Direction dir = KeyInput(); // 키 입력을 받는다.
			if (dir == CONTINUE)	// 움직임 키 외의 입력을 받으면 움직이지 않는다
				continue;
			
			if (dir == RESTART) {
				Map::RemoveAll();	// 할당받은 모든 메모리 해제
				Map::ReadMap(filename); continue;
			}
			if (dir == EXIT) {
				Map::RemoveAll();	// 할당받은 모든 메모리 해제
				system("cls");
				break;
			}

			Map::YouMove(dir);
		}
	}

	return 0;
}