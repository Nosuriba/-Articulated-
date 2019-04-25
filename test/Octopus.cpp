#include <DxLib.h>
#include "Octopus.h"
#include "Input.h"

const int distance = 60;
const int length = 50;
const int debugCnt = 20;

Octopus::Octopus()
{

	waitCnt = debugCnt;

	nodes.resize(5);

	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i]._sPoint = Vector2f(300 + (length * i), 400);
		nodes[i]._ePoint = Vector2f(300 + (length * (i + 1)), 400);
		nodes[i]._movePoint = Vector2f();
		nodes[i].cos = 0;
		nodes[i].sin = 0;
	}
}

Octopus::~Octopus()
{
	
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::Update( const Vector2f& plPos)
{
	//waitCnt--;				/// 後で使うｶｳﾝﾄ
	_pos = plPos;				/// ﾃﾞﾊﾞｯｸﾞ用	
	CalTrigonometric(_pos);		/// 三角関数の値を求めている

	Move();
	

	//debugPoint += _vel;
}

void Octopus::CalTrigonometric(const Vector2f & pos)
{
	auto A = Vector2f();
	auto B = Vector2f();

	/// 複数の計算
	for (int i = nodes.size() - 1; i >= 0; --i)
	{
		A = pos - nodes[i]._sPoint;
		B = nodes[i]._ePoint - nodes[i]._sPoint;
		A.Normalize();
		B.Normalize();

		nodes[i].cos = Dot(A, B);
		nodes[i].sin = 1.0 - abs(nodes[i].cos);
		
		nodes[i].sin = (nodes[i]._sPoint.y - pos.y > 0.f ? nodes[i].sin : -nodes[i].sin);
	}


}	

void Octopus::Move()
{
	auto d = Vector2f();

	/// 複数の移動
	for (int i = nodes.size() - 1; i >= 0; --i)
	{
		d = nodes[i]._ePoint - nodes[i]._sPoint;
		nodes[i]._movePoint.x = nodes[i]._sPoint.x + ((d.x * nodes[i].cos) - (d.y * nodes[i].sin));
		nodes[i]._movePoint.y = nodes[i]._sPoint.y - ((d.x * nodes[i].sin) + (d.y * nodes[i].cos));
	}

}

void Octopus::DebugDraw()
{
	// debug用の描画(複数の線)
	for (int i = 0; i < nodes.size(); i++)
	{
		DrawLine(nodes[i]._sPoint.x, nodes[i]._sPoint.y, nodes[i]._ePoint.x, nodes[i]._ePoint.y, 0x00ccff);
		DrawLine(nodes[i]._sPoint.x, nodes[i]._sPoint.y, nodes[i]._movePoint.x, nodes[i]._movePoint.y, 0x00ff00);

		DrawCircle(nodes[i]._ePoint.x, nodes[i]._ePoint.y, 5, 0xff8800, true);
		DrawCircle(nodes[i]._movePoint.x, nodes[i]._movePoint.y, 5, 0xffff00, true);
	}
}
