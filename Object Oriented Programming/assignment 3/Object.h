#pragma once

#include "Enum.h"
#include <string>

using namespace std;

class Object {
private:
	int x, y; //x, y 좌표
	Object* prev, * next; //이전 노드와 다음 노드
public:
	Object(int x, int y, Object* prev = nullptr, Object* next = nullptr); //생성자는 x, y좌표와 이전 노드와 다음 노드를 받는다. 자식 클래스들도 마찬가지
	void Remove(); //오브젝트가 제거됨
	void SetX(int x); //x좌표 지정
	void SetY(int y); //y좌표 지정
	int GetX(); //x좌표 리턴
	int GetY(); //y좌표 리턴
	Object* GetNext(); //다음 노드 리턴
	Object* GetPrev(); //이전 노드 리턴
	void SetNext(Object* obj); //다음 노드 지정
	void SetPrev(Object* obj); //이전 노드 지정
	virtual bool Move(Direction dir); //오브젝트가 한 칸 dir 방향으로 (이동할 수 있다면) 이동
	virtual void RuleCheck(); //맵의 문장을 읽어 각 개체에 속성을 부여한다. [Is 클래스에서 처리하는 것을 권장]
	virtual bool IsEntityText(); //Entity 클래스나 그 클래스의 자식이라면 true를 리턴
	virtual bool IsPropertyText(); //Property 클래스나 그 클래스의 자식이라면 true를 리턴
	virtual Complement GetPropertyBit(); //Text가 현재 담당하고 있는 Entity나 Property를 리턴 (Complement Enum)
	virtual int* GetPropertyPointer(); //Non-Text인 클래스에 있는 property 변수를 가리키는 포인터
	virtual int GetProperty(); //Non-Text인 클래스에 있는 property 값을 리턴
	virtual void YouMove(Direction dir); //Non-Text에서 YOU 속성을 가지고 있다면 dir 방향으로 이동 [조건 체크 후 Object::Move를 호출하는 것을 권장]
	virtual bool ChangeEntity(); //Non-Text에서 현재 property 변수를 보고 다른 개체로 바뀌어야 한다면 바꾸는 작업을 수행
	virtual string ToString(); //오브젝트의 종류에 따라 맵 상에 출력되는 string이 다르다
}; // ChangeEntity함수의 타입을 bool로 바꿈

class Baba : public Object {
private:
public:
	static int property;
	Baba(int x, int y, Object* prev, Object* next);
	static void InitRule() { //맵에서 문장을 읽기 전에 모든 오브젝트의 속성을 초기화 해야한다.
		property = NULL;
	}
	string ToString();
	int GetProperty();
	void YouMove(Direction dir);
	bool Move(Direction dir);
	bool ChangeEntity();
};

class Rock : public Object {
private:
public:
	static int property;
	Rock(int x, int y, Object* prev, Object* next);
	static void InitRule() {
		property = NULL;
	}
	string ToString();
	int GetProperty();
	void YouMove(Direction dir);
	bool Move(Direction dir);
	bool ChangeEntity();
};

class Wall : public Object {
private:
public:
	static int property;
	Wall(int x, int y, Object* prev, Object* next);
	static void InitRule() {
		property = NULL;
	}
	string ToString();
	int GetProperty();
	void YouMove(Direction dir);
	bool Move(Direction dir);
	bool ChangeEntity();
};

class Flag : public Object {
private:
public:
	static int property;
	Flag(int x, int y, Object* prev, Object* next);
	static void InitRule() {
		property = NULL;
	}
	string ToString();
	int GetProperty();
	void YouMove(Direction dir);
	bool Move(Direction dir);
	bool ChangeEntity();
};

class Text : public Object {
public:
	Text(int x, int y, Object* prev, Object* next);
	bool Move(Direction dir);
};

class Entity : public Text {
public:
	Entity(int x, int y, Object* prev, Object* next);
	bool IsEntityText();
};

class BabaText : public Entity {
public:
	BabaText(int x, int y, Object* prev, Object* next);
	int* GetPropertyPointer();
	Complement GetPropertyBit();
	string ToString();
};

class RockText : public Entity {
public:
	RockText(int x, int y, Object* prev, Object* next);
	int* GetPropertyPointer();
	Complement GetPropertyBit();
	string ToString();
};

class WallText : public Entity {
public:
	WallText(int x, int y, Object* prev, Object* next);
	int* GetPropertyPointer();
	Complement GetPropertyBit();
	string ToString();
};

class FlagText : public Entity {
public:
	FlagText(int x, int y, Object* prev, Object* next);
	int* GetPropertyPointer();
	Complement GetPropertyBit();
	string ToString();
};

class Is : public Text {
public:
	Is(int x, int y, Object* prev, Object* next);
	void RuleCheck();
	string ToString();
};

class Property : public Text {
public:
	Property(int x, int y, Object* prev, Object* next);
	bool IsPropertyText();
};

class You : public Property {
public:
	You(int x, int y, Object* prev, Object* next);
	Complement GetPropertyBit();
	string ToString();
};

class Win : public Property {
public:
	Win(int x, int y, Object* prev, Object* next);
	Complement GetPropertyBit();
	string ToString();
};

class Stop : public Property {
public:
	Stop(int x, int y, Object* prev, Object* next);
	Complement GetPropertyBit();
	string ToString();
};

class Push : public Property {
public:
	Push(int x, int y, Object* prev, Object* next);
	Complement GetPropertyBit();
	string ToString();
};

class Lose : public Property {
public:
	Lose(int x, int y, Object* prev, Object* next);
	Complement GetPropertyBit();
	string ToString();
};