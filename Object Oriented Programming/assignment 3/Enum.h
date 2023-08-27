#pragma once

#include <Windows.h>

typedef enum _arrowkey {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RESTART,
	EXIT,
	CONTINUE // default 사용을 위해 추가
} Direction;

typedef enum _complement {
	null = 0,
	YOU = 1,
	WIN = 1 << 1,
	STOP = 1 << 2,
	PUSH = 1 << 3,
	BABA = 1 << 4,
	ROCK = 1 << 5,
	WALL = 1 << 6,
	FLAG = 1 << 7,
	LOSE = 1 << 8,
} Complement;

// 콘솔 출력 커서를 입력받은 좌표로 이동시키는 함수
inline void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 입력받은 좌표를 입력받은 방향으로 한 칸 이동시키는 함수
inline void MoveXY(int& x, int& y, Direction dir) {
	switch (dir) {
	case UP:
		y -= 1;
		break;
	case DOWN:
		y += 1;
		break;
	case LEFT:
		x -= 1;
		break;
	case RIGHT:
		x += 1;
		break;
	default:
		break;
	}
}