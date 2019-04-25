#include <DxLib.h>
#include "Octopus.h"
#include "Input.h"

const int distance = 60;
const int length = 30;
const int debugCnt = 20;

Octopus::Octopus()
{
	_sPoint = Vector2f(300, 400);
	_ePoint = Vector2f(450,400);

	_pos = _vel = debugPoint = Vector2f();

	waitCnt = debugCnt;

	nodes.resize(2);

	for (int i = 0; i < nodes.size(); ++i)
	{
		nodes[i]._sPoint = Vector2f(300 + (length * i), 400);
		nodes[i]._ePoint = Vector2f(330 + (length * i), 400);
		nodes[i]._debugPoint = Vector2f();
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
	//waitCnt--;				/// ��Ŏg������
	_pos = plPos;				/// ���ޯ�ޗp	
	CalTrigonometric(_pos);		/// �O�p�֐��̒l�����߂Ă���

	Move();
	

	debugPoint += _vel;
}

void Octopus::CalTrigonometric(const Vector2f & pos)
{
	auto A = Vector2f();
	auto B = Vector2f();
	/*/// �P�̂̌v�Z	
	A =  pos - _sPoint;
	B = _ePoint - _sPoint;
	A.Normalize();
	B.Normalize();

	cos = Dot(A, B);
	sin = 1.0 - abs(cos);
	
	sin = (_sPoint.y - pos.y > 0.f ? sin : -sin);
*/
	/// �����̌v�Z
	auto node = nodes.end() - 1;
	for (; node != nodes.begin(); --node)
	{
		A = pos - (*node)._sPoint;
		B = (*node)._ePoint - (*node)._sPoint;
		A.Normalize();
		B.Normalize();

		(*node).cos = Dot(A, B);
		(*node).sin = 1.0 - abs((*node).cos);
		
		(*node).sin = ((*node)._sPoint.y - pos.y > 0.f ? (*node).sin : -(*node).sin);
	}


}	

void Octopus::Move()
{
	auto d = Vector2f();

	/// �P�̂̈ړ�
	/*d = _ePoint - _sPoint;				/// �I�_����n�_�̂܂ł̋���
	
	debugPoint.x = _sPoint.x + ((d.x * cos) - (d.y * sin));
	debugPoint.y = _sPoint.y - ((d.x * sin) + (d.y * cos));
*/
	/// �����̈ړ�
	auto node = nodes.end() - 1;
	for (; node != nodes.begin(); --node)
	{
		d = (*node)._ePoint - (*node)._sPoint;
		(*node)._debugPoint.x = (*node)._sPoint.x + ((d.x * (*node).cos) - (d.y * (*node).sin));
		(*node)._debugPoint.y = (*node)._sPoint.y - ((d.x * (*node).sin) + (d.y * (*node).cos));
	}

}

void Octopus::DebugDraw()
{
	/// debug�p�̕`��(�P�̂̐�)
	/*DrawLine(_sPoint.x, _sPoint.y, _ePoint.x, _ePoint.y, 0x00ccff);
	DrawLine(_sPoint.x, _sPoint.y, _pos.x, _pos.y, 0xff0000);
	DrawLine(_sPoint.x, _sPoint.y, debugPoint.x, debugPoint.y, 0x00ff00);

	DrawCircle(debugPoint.x, debugPoint.y, 10, 0xffff00, true);

	DrawFormatString(0, 0, 0xffffff, "cos : %f", cos);
	DrawFormatString(0, 30, 0xffffff, "sin : %f", sin);
	DrawFormatString(200, 0, 0xffffff, "�n�_      : X���W %f, Y���W %f", _sPoint.x, _sPoint.y);
	DrawFormatString(200, 30, 0xffffff, "�I�_     : X���W %f, Y���W %f", _ePoint.x, _ePoint.y);
	DrawFormatString(200, 60, 0xffffff, "CalPoint : X���W %f, Y���W %f", debugPoint.x, debugPoint.y);
	DrawFormatString(200, 90, 0xffffff, "��ڲ԰   : X���W %f, Y���W %f", _pos.x, _pos.y);*/

	// debug�p�̕`��(�����̐�)
	for (auto node : nodes)
	{
		DrawLine(node._sPoint.x, node._sPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);
		//DrawLine(node._sPoint.x, node._sPoint.y, node._debugPoint.x, node._debugPoint.y, 0x00ff00);

		DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
		DrawCircle(node._debugPoint.x, node._debugPoint.y, 5, 0xffff00, true);

	}
	//DrawLine(_sPoint.x, _sPoint.y, _pos.x, _pos.y, 0xff0000);
	DrawLine(_sPoint.x, _sPoint.y, debugPoint.x, debugPoint.y, 0x00ff00);

	DrawCircle(debugPoint.x, debugPoint.y, 10, 0xffff00, true);

	DrawFormatString(0, 0, 0xffffff, "cos : %f", cos);
	DrawFormatString(0, 30, 0xffffff, "sin : %f", sin);
	/*DrawFormatString(200, 0, 0xffffff, "�n�_    : X���W %f, Y���W %f", _sPoint.x, _sPoint.y);
	DrawFormatString(200, 30, 0xffffff, "�I�_     : X���W %f, Y���W %f", _ePoint.x, _ePoint.y);
	DrawFormatString(200, 60, 0xffffff, "CalPoint : X���W %f, Y���W %f", debugPoint.x, debugPoint.y);*/
	DrawFormatString(200, 90, 0xffffff, "��ڲ԰   : X���W %f, Y���W %f", _pos.x, _pos.y);
}
