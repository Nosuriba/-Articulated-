#include <DxLib.h>
#include "Octopus.h"
#include "Input.h"

const int distance = 60;
const int length = 80;
const int debugCnt = 20;

Octopus::Octopus()
{
	node._sPoint = Vector2f(300, 400);
	node._midPoint = Vector2f(350, 350);
	node._ePoint = Vector2f(400, 400);
	waitCnt = debugCnt;

	//nodes.resize(1);

	//for (int i = 0; i < nodes.size(); i++)
	//{
	//	nodes[i]._sPoint = Vector2f(300 + (length * i), 400);
	//	nodes[i]._midPoint = Vector2f(300 + length, 350);
	//	nodes[i]._ePoint = Vector2f(300 + (length * (i + 1)), 400);
	//	nodes[i]._movePoint = Vector2f();
	//	nodes[i].cos = 0;
	//	nodes[i].sin = 0;
	//}
}

Octopus::~Octopus()
{
	
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::Update(const Vector2f& plPos)
{
	//waitCnt--;				/// ��Ŏg������
	_pos = plPos;				/// ���ޯ�ޗp	
	CalTrigonometric(_pos);		/// �O�p�֐��̒l�����߂Ă���

	Move();
	

	node._ePoint += _vel;
}

void Octopus::CalTrigonometric(const Vector2f & pos)
{
	auto A = Vector2f();
	auto B = Vector2f();

	auto a = node._ePoint - node._sPoint;			/// �I�_����n�_
	auto b = node._ePoint - node._midPoint;			/// �I�_���璆�ԓ_
	auto c = node._midPoint - node._sPoint;			/// ���ԓ_����n�_
	a.Normalize();
	b.Normalize();
	c.Normalize();
	node.cos = (b.Magnitude() + c.Magnitude() - a.Magnitude()) / (2 * (b.Magnitude() / b.Magnitude()) * (c.Magnitude() / c.Magnitude()));
	auto debug = 0;
	/// �����̌v�Z
	/*for (int i = nodes.size() - 1; i >= 0; --i)
	{
		A = pos - nodes[i]._sPoint;
		B = nodes[i]._ePoint - nodes[i]._sPoint;
		A.Normalize();
		B.Normalize();

		nodes[i].cos = Dot(A, B);
		nodes[i].sin = 1.0 - abs(nodes[i].cos);
		
		nodes[i].sin = (nodes[i]._sPoint.y - pos.y > 0.f ? nodes[i].sin : -nodes[i].sin);
	}*/


}	

void Octopus::Move()
{
	auto d = Vector2f();

	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		_vel.x = -2;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		_vel.x = 2;
	}
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		_vel.y = -2;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		_vel.y = 2;
	}
	else
	{
		_vel = Vector2f(0, 0);
	}

	/*node._midPoint.x = */

	/*/// �����̈ړ�
	for (int i = nodes.size() - 1; i >= 0; --i)
	{
		d = nodes[i]._ePoint - nodes[i]._sPoint;
		nodes[i]._movePoint.x = nodes[i]._sPoint.x + ((d.x * nodes[i].cos) - (d.y * -nodes[i].sin));
		nodes[i]._movePoint.y = nodes[i]._sPoint.y - ((d.x * nodes[i].sin) + (d.y * nodes[i].cos));
		debugLine = nodes[i]._sPoint - nodes[i]._movePoint;
	}*/

	/*for (int i = 0; i < nodes.size() - 1; ++i)
	{
		auto r1 = _pos - nodes[i]._sPoint;
		auto r2 = _pos - nodes[i + 1]._ePoint;
		nodes[i]._ePoint.x = (r1.x * nodes[i].cos) + (r2.x * (nodes[i].cos + nodes[i + 1].cos));
		nodes[i]._ePoint.y = (r1.y * nodes[i].sin) + (r2.y * (nodes[i].sin + nodes[i + 1].sin));
	}*/

}

void Octopus::DebugDraw()
{
	// debug�p�̕`��(�����̐�)
	/*for (int i = 0; i < nodes.size(); i++)
	{
		DrawLine(nodes[i]._sPoint.x, nodes[i]._sPoint.y, nodes[i]._ePoint.x, nodes[i]._ePoint.y, 0x00ccff);
		DrawLine(nodes[i]._sPoint.x, nodes[i]._sPoint.y, nodes[i]._movePoint.x, nodes[i]._movePoint.y, 0x00ff00);

		DrawCircle(nodes[i]._ePoint.x, nodes[i]._ePoint.y, 5, 0xff8800, true);
		DrawCircle(nodes[i]._movePoint.x, nodes[i]._movePoint.y, 5, 0xffff00, true);
	}
*/
	/// ��ڲ԰�̉~�̕`��������Ă���̂ŁA�K�v�Ȏ��͌�Ŗ߂�

	//DrawLine(node._sPoint.x, node._sPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);				/// �I�_����n�_
	DrawLine(node._midPoint.x, node._midPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);				/// �I�_���璆�ԓ_
	DrawLine(node._sPoint.x, node._sPoint.y, node._midPoint.x, node._midPoint.y, 0x00ff00);			/// ���ԓ_����n�_

	DrawCircle(node._sPoint.x, node._sPoint.y, 5, 0xff0000, true);
	DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
	DrawCircle(node._midPoint.x, node._midPoint.y, 5, 0xffff00, true);

	DrawFormatString(0, 0, 0xffffff, "cos�̒l : %f", node.cos);
}
