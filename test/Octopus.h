#pragma once
#include <vector>
#include <array>
#include "Geometry.h"

struct Node
{
	Vector2f _sPoint;
	Vector2f _midPoint;
	Vector2f _ePoint;	

	float cos = 0;
	float sin = 0;
};

struct Point 
{
	Vector2f _pos;
	Vector2f _vel;
};

class Input;

class Octopus
{
public:
	Octopus();
	~Octopus();

	void Draw();
	void Update(const Vector2f& plPos);

	void Rotation();
	
private:
	void CalTrigonometric(const Vector2f& pos);			/// ŽOŠp”ä‚ÌŒvŽZ

	void DebugDraw();
	Node node;

	Vector2f _vel;
	Vector2f _pos;
};

