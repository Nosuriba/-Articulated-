#pragma once
#include "Geometry.h"

class Input;

class Player
{
public:
	Player();
	~Player();
	void Draw();
	void Update(const Input& p);
	const Vector2f GetPos();
private:
	void Move(const Input& p);

	Vector2f _pos;
	Vector2f _vel;


};

