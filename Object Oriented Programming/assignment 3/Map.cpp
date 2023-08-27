#include <iostream>
#include <fstream>
#include <string>
#include <cassert> // ���� ������
#include "Map.h"
#include "Object.h"
using namespace std;

int Map::width = 0, Map::height = 0; // static ���� �ʱ�ȭ
Object* Map::head = nullptr; // static ���� �ʱ�ȭ

bool Map::ReadMap(string str) {
	ifstream in(str); // ���� load
	string file_str;	// ���� ���� �����
	Object* temp = nullptr;
	
	head = new Object(-1, -1, nullptr, nullptr); // ���̳��� ����
	temp = head;

	if (!in.is_open()) // ���� ���� ���� ��
		return false;

	getline(in, file_str); // �� �� �б�
	width = file_str.length(); // �ʺ� = �� �� ����
	while (in) { // ���� �� ������ ����
		string s; // �ӽ� ����� ��ü
		getline(in, s);
		file_str += s; // file_str�� ��� ���� ����
	}

	int length = file_str.length();
	assert(length % width == 0); // length % width != 0 �̸� �߸��� �����̹Ƿ� ����
	height = length / width; // ���� ����

	for (int k = 0; k < width * height; k++) { // file_str���� �� ���ھ� switch case�� ���� �Ҵ� �� ����Ʈ ����
		char obj = file_str.at(k);
		int i = k % width, j = k / width; // i = x��ǥ, j = y��ǥ
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
		default: // �ƹ��͵� �Ȱɸ��� ���Ͽ� �̻��� ������ �����Ƿ� ����
			cout << "error" << endl;
			return false;
		}
	}

	return true;
}

void Map::PrintMap() {
	system("cls"); // ��� �� ȭ�� �����

	for (int j = 0; j < height; j++) { // Ʋ ���
		int y = 3 * j; // gotoxy�� �� ���� y��ǥ

		gotoxy(0, y); cout << "+";
		gotoxy(0, y + 1); cout << "|";
		gotoxy(0, y + 2); cout << "|";

		for (int i = 1; i <= width; i++) {
			int x = 3 * i; // gotoxy�� �� ���� x��ǥ

			gotoxy(x - 2, y); cout << "--+";
			gotoxy(x, y + 1); cout << "|";
			gotoxy(x, y + 2); cout << "|";
		}
	}

	gotoxy(0, 3 * height); cout << "+"; // �ٴ� �� ����� ������ ���ϹǷ� ������ֱ�
	for (int i = 0; i < width; i++) {
		cout << "--+";
	}


	for (Object* temp = head->GetNext(); temp != nullptr; temp = temp->GetNext()) { // ��� ������Ʈ ���
		int x = temp->GetX(), y = temp->GetY();
		x = 3 * x + 1; // gotoxy�� �� ���� x��ǥ
		y = 3 * y + 1; // gotoxy�� �� ���� y��ǥ

		// ��ġ�� �°� ���(non-text)
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

		// ��ġ�� �°� ���(text)
		gotoxy(x, y);
		cout << temp->ToString().substr(0, 2);
		gotoxy(x, y + 1);
		cout << temp->ToString().substr(2, 2);
	}
	
	gotoxy(0, 3 * height + 2); // Ŀ�� ��¹��� ������ ��ġ�� �ű��
}

void Map::PrintRules() {
	for (int i = 0; i < 9; i++) { // ��Ʈ���ڵ����� ������ ����Ǿ� �����Ƿ� �ִ� ���ڵ� �ڸ��� 9��° �ڸ����� Ž���ϸ鼭 �ش� �Ӽ��� ������ ���
		if (Baba::property & (YOU << i)) { // property�� YOU << i�� �Ӽ��� ������ ���� �� �ش� ���� ��� 
			cout << "BABA IS ";
			PropertyDecode(i); cout << endl; // PropertyDecode���� i�� �ִ� ��ġ�� �´� ���� ���
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
	assert(i >= 0 && i < 9); // i�� �ݵ�� 0 ~ 8�� ����
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
	while (true) { // head�� ���� ��� ��� ����
		Object* temp = head->GetNext();
		if (temp == nullptr) // head ���� ��尡 nullptr�̸� head�� ���� ����
			break;
		temp->Remove();
	}

	delete head; // head ����
	Baba::property = 0; Rock::property = 0; Wall::property = 0; Flag::property = 0; // ��� property �ʱ�ȭ
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

	while (temp != nullptr) { // ��� ������Ʈ�� ���鼭 �ش� ������Ʈ�� YouMoveȣ��
		temp->YouMove(dir);
		temp = temp->GetNext();
	}
}

bool Map::IsInBoundary(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0) // ��� �� -> false
		return false;

	return true;
}

void Map::CheckRules() {
	Object* temp = head->GetNext();

	while (temp != nullptr) { // ��� ������Ʈ�� property �ʱ�ȭ
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
	while (temp != nullptr) { // ��� ������Ʈ�� RuleCheck ����
		temp->RuleCheck();
		temp = temp->GetNext();
	}
}

bool Map::WinCheck() {
	Object* temp_win = head->GetNext();
	int x = -1, y = -1; // x, y�� ���� �� ���� ������ �ʱ�ȭ. WIN �Ӽ��� ���� ������Ʈ�� ��ǥ �����
	int xx = -2, yy = -2; // x, y�ʹ� �ٸ��鼭 x, y�� ���� �� ���� ������ �ʱ�ȭ. YOU �Ӽ��� ���� ������Ʈ�� ��ǥ �����

	while (temp_win != nullptr) {
		if (temp_win->GetProperty() & WIN) {
			x = temp_win->GetX();
			y = temp_win->GetY();
		}

		else { // WIN �Ӽ��� ������ ��� ���� �ʿ� X
			temp_win = temp_win->GetNext(); continue;
		}

		Object* temp_you = head->GetNext();
		while (temp_you != nullptr) {
			xx = temp_you->GetX();
			yy = temp_you->GetY();

			if (temp_you->GetProperty() & YOU) {
				if (xx == x && yy == y) { // �¸� ����
					RemoveAll(); // �¸��ϸ� ������ �����Ƿ� ��ü ����
					return true; // �Լ� ����
				}
			}

			temp_you = temp_you->GetNext(); // YOU �Ӽ��� ���� �ٸ� ������Ʈ�� win�� �������� ���� �����Ƿ� ��ü ����Ʈ Ž��
		}

		temp_win = temp_win->GetNext();
	}

	return false;
}

bool Map::CantMoveCheck() {
	Object* temp = head->GetNext();

	while (temp != nullptr) {
		if (temp->GetProperty() & YOU) // YOU �Ӽ��� ���� ������Ʈ�� �ִٸ� ������ �� �ִ� ����
			return false;
		temp = temp->GetNext();
	}
	
	RemoveAll(); // ���ٸ� ������ �����Ƿ� ���� ����
	return true;
}

void Map::LoseCheck() {
	Object* temp_you = head->GetNext();
	int x = -1, y = -1; // x, y�� ���� �� ���� ������ �ʱ�ȭ. YOU �Ӽ��� ���� ������Ʈ�� ��ǥ �����
	int xx = -2, yy = -2; // x, y�ʹ� �ٸ��鼭 x, y�� ���� �� ���� ������ �ʱ�ȭ. LOSE �Ӽ��� ���� ������Ʈ�� ��ǥ �����

	while (temp_you != nullptr) {
		if (temp_you->GetProperty() & YOU) {
			x = temp_you->GetX();
			y = temp_you->GetY();
		}

		else { // YOU �Ӽ��� ������ ��� ���� �ʿ� X
			temp_you = temp_you->GetNext(); continue;
		}

		Object* temp_lose = head->GetNext();
		while (temp_lose != nullptr) {
			xx = temp_lose->GetX();
			yy = temp_lose->GetY();

			if (temp_lose->GetProperty() & LOSE) {
				if (xx == x && yy == y) { // �ı� ����
					Object* temp = temp_you->GetPrev(); // ���� ����� ������ ���� ���� �����ؾ� ���� ��� ���� �� Ž���� ���� Ž�� �����ϴ�.
					temp_you->Remove(); // �ش� ��� ����
					temp_you = temp; break; // �����ߴٸ� ����Ʈ ������ Ž���ؼ��� �ȵȴ�.
				}
			}

			temp_lose = temp_lose->GetNext(); // LOSE �Ӽ��� ���� �ٸ� ������Ʈ�� you�� ��ġ�� ���� ���� �����Ƿ� ��ü ����Ʈ Ž��
		}

		temp_you = temp_you->GetNext();
	}
}

void Map::ChangeEntity() {
	Object* temp = head->GetNext();

	while (temp != nullptr) {
		if (temp->ChangeEntity()) { // ������Ʈ�� �ٲ�� ������ ������Ʈ�� �����ؾ� �Ѵ�
			Object* temp2 = temp->GetPrev(); // ���� ����� ������ ���� ���� �����ؾ� ���� ��� ���� �� Ž���� ���� Ž�� �����ϴ�.
			temp->Remove();
			temp = temp2;
		}

		assert(temp != nullptr); // nullptr�� �Ǹ� �ȵȴ�
		temp = temp->GetNext();
	}
}