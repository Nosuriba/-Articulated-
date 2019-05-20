#include <DxLib.h>
#include <cmath>
#include "Octopus.h"
#include "Input.h"

const int distance = 60;
const int length = 80;

Octopus::Octopus()
{
	node._sPoint   = Vector2f(300, 400);
	node._midPoint = Vector2f(300 + distance, 350);
	node._ePoint   = Vector2f(300 + (distance * 2), 400);
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

			/// 関節が逆になる部分をはじいている
			sinD = (node._sPoint.x > pos.x ? Vector2f(-sinD.x, -sinD.y) : 
														Vector2f(sinD.x, sinD.y));
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

void Octopus::Update(const Vector2f& plPos)
{
	CalTrigonometric(plPos);		/// 三角関数の値を求めている
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::DebugDraw()
{
	// 余弦定理を使っての関節移動
	DrawLine(node._midPoint.x, node._midPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);		/// 終点から中間点
	DrawLine(node._sPoint.x, node._sPoint.y, node._midPoint.x, node._midPoint.y, 0x00ff00);			/// 中間点から始点

	DrawCircle(node._sPoint.x, node._sPoint.y, 5, 0xff0000, true);
	DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
	DrawCircle(node._midPoint.x, node._midPoint.y, 5, 0xffff00, true);
}
