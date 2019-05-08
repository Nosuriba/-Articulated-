#include <DxLib.h>
#include "Player.h"
#include "Input.h"


Player::Player()
{
	_pos = Vector2f(300, 300);
}

Player::~Player()
{
}

void Player::Draw()
{
	DxLib::DrawCircle(_pos.x, _pos.y, 10, 0xffffff, true);
}

void Player::Update(const Input& p)
{
	Move(p);

	_pos += _vel;
}

const Vector2f Player::GetPos()
{
	return _pos;
}

void Player::Move(const Input & p)
{
	_vel = Vector2f(0, 0);
	if (p.IsPressing(PAD_INPUT_UP))
	{
		_vel.y = -5.0;
	}
	if (p.IsPressing(PAD_INPUT_DOWN))
	{
		_vel.y = 5.0;
	}
	if (p.IsPressing(PAD_INPUT_LEFT))
	{
		_vel.x = -5.0;
	}
	if (p.IsPressing(PAD_INPUT_RIGHT))
	{
		_vel.x = 5.0;
	}
}
