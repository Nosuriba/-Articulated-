#pragma once
#include <vector>
#include "Geometry.h"

struct Node
{
	Vector2f _sPoint;
	Vector2f _midPoint;
	Vector2f _ePoint;	

	Vector2f _movePoint;

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
	Vector2f _vec;	// ÍŞ¸ÄÙ‚Ì¬•ª
	float dot;		// “àÏ‚ÌŒ‹‰Ê

	void Dot(const Vector2f d)
	{
		dot = (_vec.x * d.x) + (_vec.y * d.y);
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
	void CalTrigonometric(const Vector2f& pos);			/// OŠp”ä‚ÌŒvZ
	void CalRadian(const Vector2f& pos);				/// ’¼ü‚ÌŠp“xŒvZ

	void TestMove(std::vector<Test> tNode, float vRad, float lRad, float d, Vector2f pos);

	void DebugDraw();

	std::vector<Test> testNode;
	Node node;

	Vector2f _vel;

	int waitCnt;
};

