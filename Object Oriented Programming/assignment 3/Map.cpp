#include <iostream>
#include <fstream>
#include <string>
#include <cassert> // 오류 방지용
#include "Map.h"
#include "Object.h"
using namespace std;

int Map::width = 0, Map::height = 0; // static 변수 초기화
Object* Map::head = nullptr; // static 변수 초기화

bool Map::ReadMap(string str) {
	ifstream in(str); // 파일 load
	string file_str;	// 파일 내용 저장용
	Object* temp = nullptr;
	
	head = new Object(-1, -1, nullptr, nullptr); // 더미노드로 설정
	temp = head;

	if (!in.is_open()) // 파일 열기 실패 시
		return false;

	getline(in, file_str); // 한 줄 읽기
	width = file_str.length(); // 너비 = 한 줄 길이
	while (in) { // 파일 다 읽으면 종료
		string s; // 임시 저장용 객체
		getline(in, s);
		file_str += s; // file_str에 모든 정보 저장
	}

	int length = file_str.length();
	assert(length % width == 0); // length % width != 0 이면 잘못된 파일이므로 오류
	height = length / width; // 높이 설정

	for (int k = 0; k < width * height; k++) { // file_str에서 한 글자씩 switch case로 동적 할당 및 리스트 연결
		char obj = file_str.at(k);
		int i = k % width, j = k / width; // i = x좌표, j = y좌표
		Object* node = nullptr;

		switch (obj) {
		case ' ':
			break;
		case 'B':
			node = new BabaText(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'W':
			node = new WallText(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'F':
			node = new FlagText(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'R':
			node = new RockText(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'Y':
			node = new You(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'P':
			node = new Push(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'S':
			node = new Stop(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'I':
			node = new Is(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'b':
			node = new Baba(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'w':
			node = new Wall(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'f':
			node = new Flag(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'r':
			node = new Rock(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'G':
			node = new Win(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		case 'L':
			node = new Lose(i, j, temp, nullptr);
			temp->SetNext(node);
			temp = node;
			break;
		default: // 아무것도 안걸리면 파일에 이상한 정보가 있으므로 오류
			cout << "error" << endl;
			return false;
		}
	}

	return true;
}

void Map::PrintMap() {
	system("cls"); // 출력 전 화면 지우기

	for (int j = 0; j < height; j++) { // 틀 출력
		int y = 3 * j; // gotoxy에 쓸 실제 y좌표

		gotoxy(0, y); cout << "+";
		gotoxy(0, y + 1); cout << "|";
		gotoxy(0, y + 2); cout << "|";

		for (int i = 1; i <= width; i++) {
			int x = 3 * i; // gotoxy에 쓸 실제 x좌표

			gotoxy(x - 2, y); cout << "--+";
			gotoxy(x, y + 1); cout << "|";
			gotoxy(x, y + 2); cout << "|";
		}
	}

	gotoxy(0, 3 * height); cout << "+"; // 바닥 줄 출력은 위에서 안하므로 출력해주기
	for (int i = 0; i < width; i++) {
		cout << "--+";
	}


	for (Object* temp = head->GetNext(); temp != nullptr; temp = temp->GetNext()) { // 모든 오브젝트 출력
		int x = temp->GetX(), y = temp->GetY();
		x = 3 * x + 1; // gotoxy에 쓸 실제 x좌표
		y = 3 * y + 1; // gotoxy에 쓸 실제 y좌표

		// 위치에 맞게 출력(non-text)
		if (temp->ToString().compare("b") == 0) {
			gotoxy(x, y);
			cout << "b" << endl; continue;
		}
		if (temp->ToString().compare("r") == 0) {
			gotoxy(x, y + 1);
			cout << "r" << endl; continue;
		}
		if (temp->ToString().compare("w") == 0) {
			gotoxy(x + 1, y);
			cout << "w" << endl; continue;
		}
		if (temp->ToString().compare("f") == 0) {
			gotoxy(x + 1, y + 1);
			cout << "f" << endl; continue;
		}

		// 위치에 맞게 출력(text)
		gotoxy(x, y);
		cout << temp->ToString().substr(0, 2);
		gotoxy(x, y + 1);
		cout << temp->ToString().substr(2, 2);
	}
	
	gotoxy(0, 3 * height + 2); // 커서 출력물의 마지막 위치로 옮기기
}

void Map::PrintRules() {
	for (int i = 0; i < 9; i++) { // 비트인코딩으로 정보가 저장되어 있으므로 최대 인코딩 자리인 9번째 자리까지 탐색하면서 해당 속성이 있으면 출력
		if (Baba::property & (YOU << i)) { // property가 YOU << i의 속성을 가지고 있을 때 해당 정보 출력 
			cout << "BABA IS ";
			PropertyDecode(i); cout << endl; // PropertyDecode에서 i가 있는 위치에 맞는 정보 출력
		}
		if (Rock::property & (YOU << i)) {
			cout << "ROCK IS ";
			PropertyDecode(i); cout << endl;
		}
		if (Wall::property & (YOU << i)) {
			cout << "WALL IS ";
			PropertyDecode(i); cout << endl;
		}
		if (Flag::property & (YOU << i)) {
			cout << "FLAG IS ";
			PropertyDecode(i); cout << endl;
		}
	}
}

void Map::PropertyDecode(int i) {
	assert(i >= 0 && i < 9); // i는 반드시 0 ~ 8의 정수
	switch (i) {
	case 0:
		cout << "YOU";
		break;
	case 1:
		cout << "WIN";
		break;
	case 2:
		cout << "STOP";
		break;
	case 3:
		cout << "PUSH";
		break;
	case 4:
		cout << "BABA";
		break;
	case 5:
		cout << "ROCK";
		break;
	case 6:
		cout << "WALL";
		break;
	case 7:
		cout << "FLAG";
		break;
	case 8:
		cout << "LOSE";
		break;
	}
}

void Map::RemoveAll() {
	while (true) { // head의 다음 노드 계속 삭제
		Object* temp = head->GetNext();
		if (temp == nullptr) // head 다음 노드가 nullptr이면 head만 남은 상태
			break;
		temp->Remove();
	}

	delete head; // head 삭제
	Baba::property = 0; Rock::property = 0; Wall::property = 0; Flag::property = 0; // 모든 property 초기화
}

Object* Map::GetHead() {
	return head;
}

void Map::SetHead(Object* obj) {
	head = obj;
}

int Map::GetWidth() {
	return width;
}

int Map::GetHeight() {
	return height;
}

void Map::YouMove(Direction dir) {
	Object* temp = head->GetNext();

	while (temp != nullptr) { // 모든 오브젝트를 돌면서 해당 오브젝트의 YouMove호출
		temp->YouMove(dir);
		temp = temp->GetNext();
	}
}

bool Map::IsInBoundary(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0) // 경계 밖 -> false
		return false;

	return true;
}

void Map::CheckRules() {
	Object* temp = head->GetNext();

	while (temp != nullptr) { // 모든 오브젝트의 property 초기화
		if (temp->ToString().compare("BABA") == 0)
			Baba::InitRule();
		if (temp->ToString().compare("ROCK") == 0)
			Rock::InitRule();
		if (temp->ToString().compare("WALL") == 0)
			Wall::InitRule();
		if (temp->ToString().compare("FLAG") == 0)
			Flag::InitRule();

		temp = temp->GetNext();
	}

	temp = head->GetNext();
	while (temp != nullptr) { // 모든 오브젝트의 RuleCheck 실행
		temp->RuleCheck();
		temp = temp->GetNext();
	}
}

bool Map::WinCheck() {
	Object* temp_win = head->GetNext();
	int x = -1, y = -1; // x, y가 가질 수 없는 변수로 초기화. WIN 속성을 가진 오브젝트의 좌표 저장용
	int xx = -2, yy = -2; // x, y와는 다르면서 x, y가 가질 수 없는 변수로 초기화. YOU 속성을 가진 오브젝트의 좌표 저장용

	while (temp_win != nullptr) {
		if (temp_win->GetProperty() & WIN) {
			x = temp_win->GetX();
			y = temp_win->GetY();
		}

		else { // WIN 속성이 없으면 계속 진행 필요 X
			temp_win = temp_win->GetNext(); continue;
		}

		Object* temp_you = head->GetNext();
		while (temp_you != nullptr) {
			xx = temp_you->GetX();
			yy = temp_you->GetY();

			if (temp_you->GetProperty() & YOU) {
				if (xx == x && yy == y) { // 승리 조건
					RemoveAll(); // 승리하면 게임이 끝나므로 전체 삭제
					return true; // 함수 종료
				}
			}

			temp_you = temp_you->GetNext(); // YOU 속성을 가진 다른 오브젝트가 win에 도착했을 수도 있으므로 전체 리스트 탐색
		}

		temp_win = temp_win->GetNext();
	}

	return false;
}

bool Map::CantMoveCheck() {
	Object* temp = head->GetNext();

	while (temp != nullptr) {
		if (temp->GetProperty() & YOU) // YOU 속성을 가진 오브젝트가 있다면 움직일 수 있는 상태
			return false;
		temp = temp->GetNext();
	}
	
	RemoveAll(); // 없다면 게임이 끝나므로 전부 삭제
	return true;
}

void Map::LoseCheck() {
	Object* temp_you = head->GetNext();
	int x = -1, y = -1; // x, y가 가질 수 없는 변수로 초기화. YOU 속성을 가진 오브젝트의 좌표 저장용
	int xx = -2, yy = -2; // x, y와는 다르면서 x, y가 가질 수 없는 변수로 초기화. LOSE 속성을 가진 오브젝트의 좌표 저장용

	while (temp_you != nullptr) {
		if (temp_you->GetProperty() & YOU) {
			x = temp_you->GetX();
			y = temp_you->GetY();
		}

		else { // YOU 속성이 없으면 계속 진행 필요 X
			temp_you = temp_you->GetNext(); continue;
		}

		Object* temp_lose = head->GetNext();
		while (temp_lose != nullptr) {
			xx = temp_lose->GetX();
			yy = temp_lose->GetY();

			if (temp_lose->GetProperty() & LOSE) {
				if (xx == x && yy == y) { // 파괴 조건
					Object* temp = temp_you->GetPrev(); // 현재 노드의 이전을 현재 노드로 설정해야 현재 노드 삭제 후 탐색시 정상 탐색 가능하다.
					temp_you->Remove(); // 해당 노드 제거
					temp_you = temp; break; // 제거했다면 리스트 끝까지 탐색해서는 안된다.
				}
			}

			temp_lose = temp_lose->GetNext(); // LOSE 속성을 가진 다른 오브젝트가 you와 위치가 같을 수도 있으므로 전체 리스트 탐색
		}

		temp_you = temp_you->GetNext();
	}
}

void Map::ChangeEntity() {
	Object* temp = head->GetNext();

	while (temp != nullptr) {
		if (temp->ChangeEntity()) { // 오브젝트가 바뀌면 기존의 오브젝트를 삭제해야 한다
			Object* temp2 = temp->GetPrev(); // 현재 노드의 이전을 현재 노드로 설정해야 현재 노드 삭제 후 탐색시 정상 탐색 가능하다.
			temp->Remove();
			temp = temp2;
		}

		assert(temp != nullptr); // nullptr이 되면 안된다
		temp = temp->GetNext();
	}
}