#include <iostream>
#include <string>
#include "Map.h"
#include "Object.h"

int Baba::property = 0, Rock::property = 0, Wall::property = 0, Flag::property = 0; // static ���� �ʱ�ȭ


Object::Object(int x, int y, Object* prev, Object* next) {
	this->x = x;
	this->y = y;
	this->prev = prev;
	this->next = next;
}

void Object::Remove() {
	Object* prev = this->prev, * next = this->next; // ���� ����� ����, ���� ��带 ����

	// ���� ��带 ����Ʈ���� ����(���� ����� ����, ���� ��带 ���� ����)
	prev->SetNext(next);
	if (next != nullptr)
		next->SetPrev(prev);

	delete this; // ���� ��� ����
}

void Object::SetX(int x) {
	this->x = x;
}

void Object::SetY(int y) {
	this->y = y;
}

int Object::GetX() {
	return x;
}

int Object::GetY() {
	return y;
}

Object* Object::GetNext() {
	return next;
}

Object* Object::GetPrev() {
	return prev;
}

void Object::SetNext(Object* obj) {
	next = obj;
}

void Object::SetPrev(Object* obj) {
	prev = obj;
}

bool Object::Move(Direction dir) { // ����Լ��� �ۼ�, overriding �̿�X
	int xx = x, yy = y; // �ӽ÷� ��ġ �ű�� ���� ����
	MoveXY(xx, yy, dir);
	if (!Map::IsInBoundary(xx, yy)) { // �Ű��� ������ ��� ���̸� ���� ������Ʈ�� �������� �ʴ´�.
		return false;
	}

	Object* temp = Map::GetHead()->GetNext();

	while (temp != nullptr) {
		if (temp->GetX() == xx && temp->GetY() == yy) {
			if (temp->GetProperty() & STOP) // STOP�Ӽ� ���δ� �Ű��� �� �����Ƿ�
				return false;

			else if (temp->GetProperty() & PUSH) { // �Ű��� ��ġ�� PUSH Ư���� ���� ������Ʈ�� ������ �ٽ� �� ������Ʈ�� ���� Move ����
				if (temp->Object::Move(dir)) // Move �����ϸ� ���� �ű��
					break;
				else
					return false; // STOP ��ü ���� �Ű����ų�, ��� ���� ��� ����̹Ƿ� return false
			}
		}

		temp = temp->GetNext();
	}
	
	MoveXY(x, y, dir); // ������Ʈ�� ������ �����δ�
	return true;
}

void Object::RuleCheck() {
	// IS Ư���� ���� ������Ʈ�� check �ʿ� X. IS::RuleCheck()���� ó��
}

bool Object::IsEntityText() {
	return false; // �� �Լ��� ȣ��Ǹ� �ش� ��ü�� Entitiy�� �ƴϴ�
}

bool Object::IsPropertyText() {
	return false; // �� �Լ��� ȣ��Ǹ� �ش� ��ü�� Property�� �ƴϴ�
}

Complement Object::GetPropertyBit() {
	return null;
}

int* Object::GetPropertyPointer() { // ���⼭ Property ������Ʈ�� �Ӽ��� ��ȯ�Ѵ�
	static int property = 0;
	property = this->GetPropertyBit();
	return &(property);
}

int Object::GetProperty() { // Text ������Ʈ�� ȣ��. Text ������Ʈ�� �⺻������ PUSH �Ӽ��� ������
	return PUSH;
}

void Object::YouMove(Direction dir) {
	// Text ������Ʈ���� �����ϴ� �Լ�. ������ �ʿ䰡 �����Ƿ�(Text ������Ʈ���� non-text ������Ʈ�� ���� ����������, ������ ���������� �ʴ´�), �Լ� ���� �ʿ� X
}

bool Object::ChangeEntity() {
	return false; // Text ������Ʈ���� �����ϴ� �Լ�. �ٲ� �ʿ䰡 �����Ƿ�(Text ������Ʈ���� non-text ������Ʈ�� �ٲ� �Ӽ��� �ο�����, ������ �ٲ����� �ʴ´�), �׻� �ȹٲ�� ����
}

string Object::ToString() {
	return ""; // �� ������Ʈ�� ���� overriding�� �Լ� ȣ��
}

Baba::Baba(int x, int y, Object* prev, Object* next) : Object(x, y, prev, next) {

}

string Baba::ToString() {
	return "b";
}

int Baba::GetProperty() {
	return Baba::property;
}

void Baba::YouMove(Direction dir) {
	if (Baba::property & YOU)
		Object::Move(dir); // YOU �Ӽ��� �ִٸ� ������ �� �����Ƿ� Move ȣ��
}

bool Baba::Move(Direction dir) { // ��� overriding �Լ� ��� X
	return true;
}

bool Baba::ChangeEntity() {
	if (Baba::property & BABA) // BABA �Ӽ��� ������ �ִٸ� �ٸ� ������Ʈ�� �ٲ�� �ȵǹǷ� �״�� return
		return false;

	Object* temp = nullptr, * node = nullptr;
	if (Baba::property & ROCK) {
		node = new Rock(this->GetX(), this->GetY(), this->GetPrev(), this); // ���� �Ҵ�
		this->GetPrev()->SetNext(node); // ���� ����� �ڿ� �߰��ؾ� ���α׷� ������ �߻����� �ʰ�, ���ӵ� ��Ģ�� �°� �۵��Ѵ�
		this->SetPrev(node);
		temp = node; // �� �߰��� ���� ���� ���� �����Ƿ�
	}

	if (Baba::property & WALL) { // A is B�� A is C�� ���ÿ� ������ ��� ���� ��Ģ�� ���� A�� B�� C�� �ٲ��. ���� esle if ��� if�� ����
		node = new Wall(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) { // BABA IS ROCK, BABA IS WALL�� �����ϴ� ���
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			// ���� �߿�. �ٲ�� ���� �߻�
			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else { // BABA IS WALL�� �����ϴ� ���
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
			
		}

		temp = node;
	}

	if (Baba::property & FLAG) { // ���� ���� �˰���
		node = new Flag(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (temp == node && node != nullptr) // node != nullptr�̸� ������Ʈ�� �ٲ� ����
		return true;

	return false;
}

Rock::Rock(int x, int y, Object* prev, Object* next) : Object(x, y, prev, next) {

}

string Rock::ToString() {
	return "r";
}

int Rock::GetProperty() {
	return Rock::property;
}

void Rock::YouMove(Direction dir) { // Baba::YouMove(Direction dir)�� ���� �˰���
	if (Rock::property & YOU)
		Object::Move(dir);
}

bool Rock::Move(Direction dir) { // Baba::Move(Direction dir)�� ���� �˰���
	return true;
}

bool Rock::ChangeEntity() { // Baba::ChangeEntity()�� ���� �˰���
	if (Rock::property & ROCK)
		return false;

	Object* temp = nullptr, * node = nullptr;
	if (Rock::property & BABA) {
		node = new Baba(this->GetX(), this->GetY(), this->GetPrev(), this);
		this->GetPrev()->SetNext(node);
		this->SetPrev(node);
		temp = node;
	}

	if (Rock::property & WALL) {
		node = new Wall(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (Rock::property & FLAG) {
		node = new Flag(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (temp == node && node != nullptr)
		return true;

	return false;
}

Wall::Wall(int x, int y, Object* prev, Object* next) : Object(x, y, prev, next) {

}

string Wall::ToString() {
	return "w";
}

int Wall::GetProperty() {
	return Wall::property;
}

void Wall::YouMove(Direction dir) { // Baba::YouMove(Direction dir)�� ���� �˰���
	if (Wall::property & YOU)
		Object::Move(dir);
}

bool Wall::Move(Direction dir) { // Baba::Move(Direction dir)�� ���� �˰���
	return true;
}

bool Wall::ChangeEntity() { // Baba::ChangeEntity()�� ���� �˰���
	if (Wall::property & WALL)
		return false;
	
	Object* temp = nullptr, * node = nullptr;
	if (Wall::property & BABA) {
		node = new Baba(this->GetX(), this->GetY(), this->GetPrev(), this);
		this->GetPrev()->SetNext(node);
		this->SetPrev(node);
		temp = node;
	}

	if (Wall::property & ROCK) {
		node = new Rock(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (Wall::property & FLAG) {
		node = new Flag(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (temp == node && node != nullptr)
		return true;

	return false;
}

Flag::Flag(int x, int y, Object* prev, Object* next) : Object(x, y, prev, next) {

}

string Flag::ToString() {
	return "f";
}

int Flag::GetProperty() {
	return Flag::property;
}

void Flag::YouMove(Direction dir) { // Baba::YouMove(Direction dir)�� ���� �˰���
	if (Flag::property & YOU)
		Object::Move(dir);
}

bool Flag::Move(Direction dir) { // Baba::YouMove(Direction dir)�� ���� �˰���
	return true;
}

bool Flag::ChangeEntity() { // Baba::ChangeEntity()�� ���� �˰���
	if (Flag::property & FLAG)
		return false;

	Object* temp = nullptr, * node = nullptr;
	if (Flag::property & BABA) {
		node = new Baba(this->GetX(), this->GetY(), this->GetPrev(), this);
		this->GetPrev()->SetNext(node);
		this->SetPrev(node);
		temp = node;
	}

	if (Flag::property & ROCK) {
		node = new Rock(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (Flag::property & WALL) {
		node = new Wall(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) {
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else {
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
		}

		temp = node;
	}

	if (temp == node && node != nullptr)
		return true;

	return false;
}

Text::Text(int x, int y, Object* prev, Object* next) : Object(x, y, prev, next) {

}

bool Text::Move(Direction dir) { // Baba::Move(Direction dir)�� ���� �˰���
	return true;
}

Entity::Entity(int x, int y, Object* prev, Object* next) : Text(x, y, prev, next) {

}

bool Entity::IsEntityText() {
	return true;
}

BabaText::BabaText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* BabaText::GetPropertyPointer() {
	return &Baba::property; // BABA �Ӽ��� ���� property ���� ��ȯ
}

Complement BabaText::GetPropertyBit() {
	return BABA; // BABA �Ӽ� ��ȯ
}

string BabaText::ToString() {
	return "BABA";
}

RockText::RockText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* RockText::GetPropertyPointer() { // BabaText::GetPropertyPointer()�� ���� �˰���
	return &Rock::property;
}

Complement RockText::GetPropertyBit() { // BabaText::GetPropertyBit()�� ���� �˰���
	return ROCK;
}

string RockText::ToString() {
	return "ROCK";
}

WallText::WallText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* WallText::GetPropertyPointer() { // BabaText::GetPropertyPointer()�� ���� �˰���
	return &Wall::property;
}

Complement WallText::GetPropertyBit() { // BabaText::GetPropertyBit()�� ���� �˰���
	return WALL;
}

string WallText::ToString() {
	return "WALL";
}

FlagText::FlagText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* FlagText::GetPropertyPointer() { // BabaText::GetPropertyPointer()�� ���� �˰���

	return &Flag::property;
}

Complement FlagText::GetPropertyBit() { // BabaText::GetPropertyBit()�� ���� �˰���
	return FLAG;
}

string FlagText::ToString() {
	return "FLAG";
}

Is::Is(int x, int y, Object* prev, Object* next) : Text(x, y, prev, next) {
	//�����
}

void Is::RuleCheck() {
	Object* temp = Map::GetHead()->GetNext();
	int x = this->GetX(), y = this->GetY();
	Object* x_subject = nullptr, * y_subject = nullptr, * x_complement = nullptr, * y_complement = nullptr;
	
	while (temp != nullptr) { // IS ������Ʈ�� �������� �����¿쿡 text ������Ʈ(IS ����)�� �ִ��� Ȯ���� ���� ����
		int xx = temp->GetX(), yy = temp->GetY();
		if ((xx == x - 1) && (yy == y) && (temp->IsEntityText() || temp->IsPropertyText()))
			x_subject = temp;
		else if ((xx == x) && (yy == y - 1) && (temp->IsEntityText() || temp->IsPropertyText()))
			y_subject = temp;
		else if ((xx == x + 1) && (yy == y) && (temp->IsEntityText() || temp->IsPropertyText()))
			x_complement = temp;
		else if ((xx == x) && (yy == y + 1) && (temp->IsEntityText() || temp->IsPropertyText()))
			y_complement = temp;

		temp = temp->GetNext();
	}

	if (x_subject != nullptr && x_complement != nullptr && x_subject->IsEntityText()) { // �־ property text�� ������ �ȵȴ�
		if (x_complement->IsEntityText()) // ��� entity text�̸� �ش� ������Ʈ �̸��� �߰�(ex. BABA IS WALL �̸� Baba::property�� WALL�� �߰�)
				*(x_subject->GetPropertyPointer()) |= x_complement->GetPropertyBit();
		*(x_subject->GetPropertyPointer()) |= *(x_complement->GetPropertyPointer()); // �־��� �Ӽ��� ������ �Ӽ� �߰�
	}

	if (y_subject != nullptr && y_complement != nullptr && y_subject->IsEntityText()) { // ���� ���� �˰���
		if (y_complement->IsEntityText())
			*(y_subject->GetPropertyPointer()) |= y_complement->GetPropertyBit();
		*(y_subject->GetPropertyPointer()) |= *(y_complement->GetPropertyPointer());
	}
}

string Is::ToString() {
	return "I  S";
}

Property::Property(int x, int y, Object* prev, Object* next) : Text(x, y, prev, next) {

}

bool Property::IsPropertyText() {
	return true; // �� �Լ��� ȣ���ϴ� ��ü�� Property ������Ʈ�̴�
}

You::You(int x, int y, Object* prev, Object* next) : Property(x, y, prev, next) {
	
}

Complement You::GetPropertyBit() {
	return YOU;
}

string You::ToString() {
	return "YO U";
}

Win::Win(int x, int y, Object* prev, Object* next) : Property(x, y, prev, next) {

}

Complement Win::GetPropertyBit() {
	return WIN;
}

string Win::ToString() {
	return "WI N";
}

Stop::Stop(int x, int y, Object* prev, Object* next) : Property(x, y, prev, next) {

}

Complement Stop::GetPropertyBit() {
	return STOP;
}

string Stop::ToString() {
	return "STOP";
}

Push::Push(int x, int y, Object* prev, Object* next) : Property(x, y, prev, next) {

}

Complement Push::GetPropertyBit() {
	return PUSH;
}

string Push::ToString() {
	return "PUSH";
}

Lose::Lose(int x, int y, Object* prev, Object* next) : Property(x, y, prev, next) {

}

Complement Lose::GetPropertyBit() {
	return LOSE;
}

string Lose::ToString() {
	return "LOSE";
}