#pragma once
#include <vector>
#include "Geometry.h"

struct Node
{
	Vector2f _sPoint;
	Vector2f _midPoint;
	Vector2f _ePoint;	

	float cos = 0;
	float sin = 0;
};

struct Test
{
	Vector2f _pos = Vector2f();
	float _rad = 0;

	Test(const Vector2f& pos, const float& rad) : _pos(pos), _rad(rad) {};
};

struct TestVec
{
	Vector2f _vec;	// �޸�ق̐���
	float dot;		// ���ς̌���

	void Dot(const Vector2f d)
	{
		dot = (_vec.x * d.x) + (_vec.y * d.y);
		auto debug = 0;
	}
};

class Input;

class Octopus
{
public:
	Octopus();
	~Octopus();

	void Draw();
	void Update(const Vector2f& plPos);
	
private:
	void CalTrigonometric(const Vector2f& pos);			/// �O�p��̌v�Z

	void DebugDraw();
	Node node;

	Vector2f _vel;

	int waitCnt;
};

