#include <DxLib.h>
#include <cmath>
#include "Octopus.h"
#include "Input.h"

const int distance = 60;
const int length = 80;
const int debugCnt = 20;

Octopus::Octopus()
{
	node._sPoint   = Vector2f(300, 400);
	node._midPoint = Vector2f(300 + distance, 350);
	node._ePoint   = Vector2f(300 + (distance * 2), 400);

	for (int i = 0; i < point.size(); ++i)
	{
		point[i]._vel = Vector2f(0, 0);
		point[i]._strgPos = Vector2f(0, 0);
		point[i]._pos = Vector2f(300 + (distance * i), 400);
	}
	waitCnt = debugCnt;
}

Octopus::~Octopus()
{
	
}

void Octopus::CalTrigonometric(const Vector2f & pos)
{
	///余弦定理
	auto pLength = pos - node._sPoint;

	node.cos = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
	auto rad = acos(node.cos);			/// cosの角度
	node.sin = sin(rad);				/// sinの値
	
	/// 値が異常値になった時の初期化
	if (!(std::isnan(node.cos) && std::isnan(node.sin)))
	{
		if (pLength.Magnitude() < length * 2)
		{
			/// 長さが一定距離より短くなった時の処理

			/// 外積を使って、Y方向の単位ﾍﾞｸﾄﾙを求めている
			auto cross = Cross(Vector3f(pLength.x, pLength.y, 0), Vector3f(0, 0, 1));
			auto cross2f = Vector2f(cross.x, cross.y);
			cross2f.Normalize();

			auto cosD = pLength.Normalized() * (length * node.cos);		/// X方向の成分
			auto sinD = cross2f * (length * node.sin);					/// Y方向の成分

			node._midPoint = node._sPoint + cosD + sinD;
			node._ePoint = pos;
		}
		else
		{
			/// 長さが一定距離よりながくなった場合
			node._midPoint = node._sPoint + (pLength.Normalized() * length);
			node._ePoint   = node._midPoint + (pLength.Normalized() * length);
		}
	}
	
	else
	{
		node.cos = 0.f;
		node.sin = 0.f;
	}
	
}

void Octopus::CalPos(const Vector2f & pos)
{
	if (point[0]._pos.x < pos.x && point[2]._pos.x > pos.x)
	{
		return;
	}

	auto plPos = pos;
	if (point[0]._pos.x > pos.x)
	{
		Vector2f sDir, mDir, calPos;

		sDir = plPos - point[0]._pos;
		sDir.Normalize();

		calPos = point[0]._pos + Vector2f(length * sDir.x, length * sDir.y);
		strgPos[0] = calPos;
		point[0]._vel = Vector2f(1.0f * sDir.x, 1.0f * sDir.y);

		mDir = point[1]._pos - calPos;
		mDir.Normalize();
		calPos = point[1]._pos + Vector2f(length * mDir.x, length * mDir.y);
		strgPos[2] = calPos;
		return;

	}
	else
	{
		Vector2f eDir, mDir, calPos;

		eDir = plPos - point[2]._pos;
		eDir.Normalize();

		calPos = point[2]._pos + Vector2f(length * eDir.x, length * eDir.y);
		strgPos[2] = calPos;
		point[2]._vel = Vector2f(1.0f * eDir.x, 1.0f * eDir.y);

		mDir = point[1]._pos - calPos;
		mDir.Normalize();
		calPos = point[1]._pos + Vector2f(length * mDir.x, length * mDir.y);
		strgPos[0] = calPos;
		return;
	}
}

void Octopus::Update(const Vector2f& plPos)
{
	//waitCnt--;					/// 後で使うｶｳﾝﾄ	
	// CalTrigonometric(plPos);		/// 三角関数の値を求めている
	if (strgPos.size() <= 0)
	{
		CalPos(plPos);
	}
	else
	{
		for (int i = 0; i < strgPos.size(); ++i)
		{
			point[i]._pos += point[i]._vel;
		}
	}
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::DebugDraw()
{
	// 余弦定理を使っての関節移動
	/*DrawLine(node._midPoint.x, node._midPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);		/// 終点から中間点
	DrawLine(node._sPoint.x, node._sPoint.y, node._midPoint.x, node._midPoint.y, 0x00ff00);			/// 中間点から始点

	DrawCircle(node._sPoint.x, node._sPoint.y, 5, 0xff0000, true);
	DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
	DrawCircle(node._midPoint.x, node._midPoint.y, 5, 0xffff00, true);*/

	for (int i = 1; i < point.size(); ++i)
	{
		DrawLine(point[i - 1]._pos.x, point[i - 1]._pos.y, point[i]._pos.x, point[i]._pos.y, 0x00ff00, 2.5f);
		DrawCircle(point[i]._pos.x, point[i]._pos.y, 5, 0xff8800, true);
		
	}
	DrawCircle(point[0]._pos.x, point[0]._pos.y, 5, 0xff8800, true);

	DrawFormatString(0, 0, 0xffffff, "cosの値 : %f", node.cos);
	DrawFormatString(0, 30, 0xffffff, "sinの値 : %f", node.sin);
}
