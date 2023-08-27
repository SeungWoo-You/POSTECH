#include <iostream>
#include <string>
#include "Map.h"
#include "Object.h"

int Baba::property = 0, Rock::property = 0, Wall::property = 0, Flag::property = 0; // static 변수 초기화


Object::Object(int x, int y, Object* prev, Object* next) {
	this->x = x;
	this->y = y;
	this->prev = prev;
	this->next = next;
}

void Object::Remove() {
	Object* prev = this->prev, * next = this->next; // 현재 노드의 이전, 다음 노드를 지정

	// 현재 노드를 리스트에서 빼기(현재 노드의 이전, 다음 노드를 서로 연결)
	prev->SetNext(next);
	if (next != nullptr)
		next->SetPrev(prev);

	delete this; // 현재 노드 삭제
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

bool Object::Move(Direction dir) { // 재귀함수로 작성, overriding 이용X
	int xx = x, yy = y; // 임시로 위치 옮기기 위해 설정
	MoveXY(xx, yy, dir);
	if (!Map::IsInBoundary(xx, yy)) { // 옮겨진 지점이 경계 밖이면 실제 오브젝트는 움직이지 않는다.
		return false;
	}

	Object* temp = Map::GetHead()->GetNext();

	while (temp != nullptr) {
		if (temp->GetX() == xx && temp->GetY() == yy) {
			if (temp->GetProperty() & STOP) // STOP속성 위로는 옮겨질 수 없으므로
				return false;

			else if (temp->GetProperty() & PUSH) { // 옮겨진 위치에 PUSH 특성을 가진 오브젝트가 있으면 다시 그 오브젝트에 대해 Move 실행
				if (temp->Object::Move(dir)) // Move 성공하면 전부 옮기기
					break;
				else
					return false; // STOP 개체 위로 옮겨졌거나, 경계 밖을 벗어난 경우이므로 return false
			}
		}

		temp = temp->GetNext();
	}
	
	MoveXY(x, y, dir); // 오브젝트를 실제로 움직인다
	return true;
}

void Object::RuleCheck() {
	// IS 특성이 없는 오브젝트는 check 필요 X. IS::RuleCheck()에서 처리
}

bool Object::IsEntityText() {
	return false; // 이 함수가 호출되면 해당 객체는 Entitiy가 아니다
}

bool Object::IsPropertyText() {
	return false; // 이 함수가 호출되면 해당 객체는 Property가 아니다
}

Complement Object::GetPropertyBit() {
	return null;
}

int* Object::GetPropertyPointer() { // 여기서 Property 오브젝트의 속성을 반환한다
	static int property = 0;
	property = this->GetPropertyBit();
	return &(property);
}

int Object::GetProperty() { // Text 오브젝트가 호출. Text 오브젝트는 기본적으로 PUSH 속성을 가진다
	return PUSH;
}

void Object::YouMove(Direction dir) {
	// Text 오브젝트들이 실행하는 함수. 움직일 필요가 없으므로(Text 오브젝트들은 non-text 오브젝트에 의해 움직여지지, 스스로 움직이지는 않는다), 함수 내용 필요 X
}

bool Object::ChangeEntity() {
	return false; // Text 오브젝트들이 실행하는 함수. 바뀔 필요가 없으므로(Text 오브젝트들은 non-text 오브젝트에 바뀔 속성을 부여하지, 스스로 바뀌지는 않는다), 항상 안바뀌는 상태
}

string Object::ToString() {
	return ""; // 각 오브젝트가 서로 overriding된 함수 호출
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
		Object::Move(dir); // YOU 속성이 있다면 움직일 수 있으므로 Move 호출
}

bool Baba::Move(Direction dir) { // 모든 overriding 함수 사용 X
	return true;
}

bool Baba::ChangeEntity() {
	if (Baba::property & BABA) // BABA 속성을 가지고 있다면 다른 오브젝트로 바뀌면 안되므로 그대로 return
		return false;

	Object* temp = nullptr, * node = nullptr;
	if (Baba::property & ROCK) {
		node = new Rock(this->GetX(), this->GetY(), this->GetPrev(), this); // 새로 할당
		this->GetPrev()->SetNext(node); // 현재 노드의 뒤에 추가해야 프로그램 오류가 발생하지 않고, 게임도 규칙에 맞게 작동한다
		this->SetPrev(node);
		temp = node; // 더 추가할 일이 있을 수도 있으므로
	}

	if (Baba::property & WALL) { // A is B와 A is C가 동시에 존재할 경우 게임 규칙에 따라 A는 B와 C로 바뀐다. 따라서 esle if 대신 if로 설정
		node = new Wall(this->GetX(), this->GetY(), nullptr, nullptr);
		if (temp != nullptr) { // BABA IS ROCK, BABA IS WALL이 존재하는 경우
			node->SetNext(temp);
			node->SetPrev(temp->GetPrev());

			// 순서 중요. 바뀌면 오류 발생
			temp->GetPrev()->SetNext(node);
			temp->SetPrev(node);
		}

		else { // BABA IS WALL만 존재하는 경우
			node->SetPrev(this->GetPrev());
			node->SetNext(this);

			this->GetPrev()->SetNext(node);
			this->SetPrev(node);
			
		}

		temp = node;
	}

	if (Baba::property & FLAG) { // 위랑 같은 알고리즘
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

	if (temp == node && node != nullptr) // node != nullptr이면 오브젝트가 바뀐 상태
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

void Rock::YouMove(Direction dir) { // Baba::YouMove(Direction dir)과 같은 알고리즘
	if (Rock::property & YOU)
		Object::Move(dir);
}

bool Rock::Move(Direction dir) { // Baba::Move(Direction dir)과 같은 알고리즘
	return true;
}

bool Rock::ChangeEntity() { // Baba::ChangeEntity()과 같은 알고리즘
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

void Wall::YouMove(Direction dir) { // Baba::YouMove(Direction dir)과 같은 알고리즘
	if (Wall::property & YOU)
		Object::Move(dir);
}

bool Wall::Move(Direction dir) { // Baba::Move(Direction dir)과 같은 알고리즘
	return true;
}

bool Wall::ChangeEntity() { // Baba::ChangeEntity()과 같은 알고리즘
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

void Flag::YouMove(Direction dir) { // Baba::YouMove(Direction dir)과 같은 알고리즘
	if (Flag::property & YOU)
		Object::Move(dir);
}

bool Flag::Move(Direction dir) { // Baba::YouMove(Direction dir)과 같은 알고리즘
	return true;
}

bool Flag::ChangeEntity() { // Baba::ChangeEntity()과 같은 알고리즘
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

bool Text::Move(Direction dir) { // Baba::Move(Direction dir)과 같은 알고리즘
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
	return &Baba::property; // BABA 속성을 가진 property 변수 반환
}

Complement BabaText::GetPropertyBit() {
	return BABA; // BABA 속성 반환
}

string BabaText::ToString() {
	return "BABA";
}

RockText::RockText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* RockText::GetPropertyPointer() { // BabaText::GetPropertyPointer()와 같은 알고리즘
	return &Rock::property;
}

Complement RockText::GetPropertyBit() { // BabaText::GetPropertyBit()와 같은 알고리즘
	return ROCK;
}

string RockText::ToString() {
	return "ROCK";
}

WallText::WallText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* WallText::GetPropertyPointer() { // BabaText::GetPropertyPointer()와 같은 알고리즘
	return &Wall::property;
}

Complement WallText::GetPropertyBit() { // BabaText::GetPropertyBit()와 같은 알고리즘
	return WALL;
}

string WallText::ToString() {
	return "WALL";
}

FlagText::FlagText(int x, int y, Object* prev, Object* next) : Entity(x, y, prev, next) {

}

int* FlagText::GetPropertyPointer() { // BabaText::GetPropertyPointer()와 같은 알고리즘

	return &Flag::property;
}

Complement FlagText::GetPropertyBit() { // BabaText::GetPropertyBit()와 같은 알고리즘
	return FLAG;
}

string FlagText::ToString() {
	return "FLAG";
}

Is::Is(int x, int y, Object* prev, Object* next) : Text(x, y, prev, next) {
	//빈공간
}

void Is::RuleCheck() {
	Object* temp = Map::GetHead()->GetNext();
	int x = this->GetX(), y = this->GetY();
	Object* x_subject = nullptr, * y_subject = nullptr, * x_complement = nullptr, * y_complement = nullptr;
	
	while (temp != nullptr) { // IS 오브젝트를 기준으로 상하좌우에 text 오브젝트(IS 제외)가 있는지 확인해 따로 저장
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

	if (x_subject != nullptr && x_complement != nullptr && x_subject->IsEntityText()) { // 주어에 property text가 들어오면 안된다
		if (x_complement->IsEntityText()) // 보어가 entity text이면 해당 오브젝트 이름을 추가(ex. BABA IS WALL 이면 Baba::property에 WALL을 추가)
				*(x_subject->GetPropertyPointer()) |= x_complement->GetPropertyBit();
		*(x_subject->GetPropertyPointer()) |= *(x_complement->GetPropertyPointer()); // 주어의 속성에 보어의 속성 추가
	}

	if (y_subject != nullptr && y_complement != nullptr && y_subject->IsEntityText()) { // 위랑 같은 알고리즘
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
	return true; // 이 함수를 호출하는 객체는 Property 오브젝트이다
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