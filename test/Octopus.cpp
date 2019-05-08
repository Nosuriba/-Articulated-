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
	waitCnt = debugCnt;

	for (int i = 0; i < 3; ++i)
	{
		/// IK用の関節を追加している
		testNode.push_back(Test(Vector2f(300 + (distance * i), 400), 90));
	}
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
void Octopus::CalRadian(const Vector2f & pos)
{
	auto d = pos - testNode[0]._pos;
	auto tan2 = atan2(d.y, d.x);

	testNode[0]._rad = tan2 * 180.0 / DX_PI;
}

//// 関節の移動(IKのテスト用関数)
/// @param tNode 関節
/// @param vRad  関節の回転角度
/// @param lRad 回転角度の限界値
/// @param distance 関節間の距離
/// @param pos 目標座標
void Octopus::TestMove(std::vector<Test> tNode, float vRad, float lRad, float d, Vector2f pos)
{
	float cosT = cos(vRad);
	float sinT = sin(vRad);

	/// 回転角度の計算
	Test nTip = testNode[testNode.size() - 1];		/// 先端の指定
	for (int i = testNode.size() - 1; i >= 0; --i)
	{
		Test n = testNode[i];
		
		/// 関節から自機へのﾍﾞｸﾄﾙ
		float dx = pos.x - n._pos.x;
		float dy = pos.y - n._pos.y;

		/// 関節から先端ﾍﾞｸﾄﾙへの内積の計算
		TestVec jNode;
		jNode._vec.x = nTip._pos.x - n._pos.x;
		jNode._vec.y = nTip._pos.y - n._pos.y;
		jNode.Dot(Vector2f(dx, dy));

		/// 右回転ﾍﾞｸﾄﾙの計算
		TestVec right;
		if (n._rad + vRad <= lRad)
		{
			right._vec.x = cosT * jNode._vec.x - sinT * jNode._vec.y;
			right._vec.y = sinT * jNode._vec.x + cosT * jNode._vec.y;
			right.Dot(Vector2f(dx, dy));
		}
		else
		{
			/// 回転ができない時
			right.dot = jNode.dot;
		}

		/// 左回転のﾍﾞｸﾄﾙの計算
		TestVec left;
		if (n._rad - vRad >= -lRad)
		{
			left._vec.x = cosT * jNode._vec.x + sinT * jNode._vec.y;
			left._vec.y = -sinT * jNode._vec.y + cosT * jNode._vec.y;
			left.Dot(Vector2f(dx, dy));
		}
		else
		{
			/// 回転ができない時
			left.dot = jNode.dot;
		}

		/// 回転方向の選択
		if (right.dot > jNode.dot && right.dot > left.dot)
		{
			/// 右回転
			n._rad += vRad;
			nTip._pos.x = n._pos.x + right._vec.x;
			nTip._pos.y = n._pos.y + right._vec.y;
		}

		if (left.dot > jNode.dot && left.dot > right.dot)
		{
			/// 左回転
			n._rad -= vRad;
			nTip._pos.x = n._pos.x + left._vec.x;
			nTip._pos.y = n._pos.y + left._vec.y;
		}
	}

	/// 座標の計算
	float px = d;
	float py = 0;

	for (int i = 1; i < testNode.size(); ++i)
	{
		Test n1 = testNode[i - 1];
		Test n2 = testNode[i];
		
		float cosT = cos(n1._rad);
		float sinT = sin(n1._rad);
		float dx = cosT * px - sinT * py;
		float dy = sinT * px + cosT * py;

		n2._pos.x = n1._pos.x + dx;
		n2._pos.y = n1._pos.y + dy;
		px = dx;
		py = dy;

		testNode[i]._pos = n2._pos;
	}
}

void Octopus::Update(const Vector2f& plPos)
{
	//waitCnt--;					/// 後で使うｶｳﾝﾄ	
	CalTrigonometric(plPos);		/// 三角関数の値を求めている
	CalRadian(plPos);
	TestMove(testNode, testNode[0]._rad, 180, distance, plPos);
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::DebugDraw()
{
	/// 余弦定理を使っての関節移動
	/*DrawLine(node._midPoint.x, node._midPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);		/// 終点から中間点
	DrawLine(node._sPoint.x, node._sPoint.y, node._midPoint.x, node._midPoint.y, 0x00ff00);			/// 中間点から始点

	DrawCircle(node._sPoint.x, node._sPoint.y, 5, 0xff0000, true);
	DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
	DrawCircle(node._midPoint.x, node._midPoint.y, 5, 0xffff00, true);

	DrawFormatString(0, 0, 0xffffff, "cosの値 : %f", node.cos);
	DrawFormatString(0, 30, 0xffffff, "sinの値 : %f", node.sin);*/

	/// IKを使っての関節移動
	for (int i = 1; i < testNode.size(); ++i)
	{
		auto sPos = testNode[i - 1]._pos;
		auto ePos = testNode[i]._pos;
		DrawLine(sPos.x, sPos.y, ePos.x, ePos.y, 0xffff00);
		DrawCircle(sPos.x, sPos.y, 5, 0xffffff, true);			/// 始点の円
		DrawCircle(ePos.x, ePos.y, 5, 0xffffff, true);			/// 終点の円

	}
}
