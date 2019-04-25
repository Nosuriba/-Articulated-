#include <DxLib.h>
#include "Octopus.h"
#include "Input.h"

const int distance = 60;
const int debugCnt = 20;

Octopus::Octopus()
{
	_sPoint = Vector2f(300, 400);
	_ePoint = Vector2f(450,400);

	_pos = _vel = Vector2f(0, 0);

	waitCnt = debugCnt;
}

Octopus::~Octopus()
{
	
}

void Octopus::Draw()
{
	DrawLine(_sPoint.x, _sPoint.y, _ePoint.x, _ePoint.y, 0xffff00);
	DrawLine(_sPoint.x, _sPoint.y, _pos.x, _pos.y, 0xff0000);
	DrawLine(_sPoint.x, _sPoint.y, debugPoint.x, debugPoint.y, 0x00ff00);

	DrawFormatString(0, 0,  0xffffff, "cos : %f", cos);
	DrawFormatString(0, 30, 0xffffff, "sin : %f", sin);
	DrawFormatString(200, 0 , 0xffffff, "�n�_ : X���W %f, Y���W %f", _sPoint.x, _sPoint.y);
	DrawFormatString(200, 30, 0xffffff, "�I�_ : X���W %f, Y���W %f", _ePoint.x, _ePoint.y);
	DrawFormatString(200, 60, 0xffffff, "CalPoint : X���W %f, Y���W %f", debugPoint.x, debugPoint.y);
	DrawFormatString(200, 90, 0xffffff, "��ڲ԰ : X���W %f, Y���W %f", _pos.x, _pos.y);
	
	// DrawFormatString(0, 60, 0xffffff, "tan : %f", tan);
}

void Octopus::Update( const Vector2f& plPos)
{
	//waitCnt--;				/// ��Ŏg������
	_pos = plPos;				/// ���ޯ�ޗp	
	CalTrigonometric(_pos);

	Move();
	

	_ePoint += _vel;
}

void Octopus::CalTrigonometric(const Vector2f & pos)
{
	auto A =  pos - _sPoint;
	auto B = _ePoint - _sPoint;
	A.Normalize();
	B.Normalize();

	cos = Dot(A, B);
	sin = 1.0 - abs(cos);
	
	sin = (_sPoint.y - pos.y > 0.f ? sin : -sin);

}	

void Octopus::Move()
{
	auto distance = _ePoint - _sPoint;				/// �I�_����n�_�̂܂ł̋���
	
	debugPoint.x = _sPoint.x + ((distance.x * cos) - (distance.y * sin));
	debugPoint.y = _sPoint.y - ((distance.x * sin) + (distance.y * cos));
}
