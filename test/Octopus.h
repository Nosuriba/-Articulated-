#pragma once
#include <vector>
#include "Geometry.h"

struct Node
{
	Vector2f _sPoint;
	Vector2f _ePoint;
	Vector2f _debugPoint;

	float cos = 0;
	float sin = 0;
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
	void CalTrigonometric(const Vector2f& pos);			/// 三角比の計算
	void Move();

	void DebugDraw();

	std::vector<Node> nodes;

	Vector2f _sPoint;
	Vector2f _ePoint;
	Vector2f debugPoint;	//　後で消す

	Vector2f _pos;		// とりあえず、ﾌﾟﾚｲﾔｰ用の座標
	Vector2f _vel;

	int waitCnt;

	float cos = 0;
	float sin = 0;
};

