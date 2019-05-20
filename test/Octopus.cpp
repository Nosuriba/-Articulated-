#include <DxLib.h>
#include <cmath>
#include "Octopus.h"
#include "Input.h"
#include "Game.h"

const int distance = 60;
const int length = 80;

const VECTOR vec1 = { 1.f, 0.f, 0.f };
const VECTOR vec2 = { 0.f, 0.f, 0.f };
const VECTOR vec3 = { 0.f, 0.f, -1.0f };
const float rad = DX_PI / 180.f;

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
	///�]���藝
	auto pLength = pos - node._sPoint;

	node.cos = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
	auto rad = acos(node.cos);			/// cos�̊p�x
	node.sin = sin(rad);				/// sin�̒l
	
	/// �l���ُ�l�ɂȂ������̏�����
	if (!(std::isnan(node.cos) && std::isnan(node.sin)))
	{
		if (pLength.Magnitude() < length * 2)
		{
			/// ��������苗�����Z���Ȃ������̏���

			/// �O�ς��g���āAY�����̒P���޸�ق����߂Ă���
			auto cross = Cross(Vector3f(pLength.x, pLength.y, 0), Vector3f(0, 0, 1));
			auto cross2f = Vector2f(cross.x, cross.y);
			cross2f.Normalize();

			auto cosD = pLength.Normalized() * (length * node.cos);		/// X�����̐���
			auto sinD = cross2f * (length * node.sin);					/// Y�����̐���

			/// �֐߂��t�ɂȂ镔�����͂����Ă���
			sinD = (node._sPoint.x > pos.x ? Vector2f(-sinD.x, -sinD.y) : 
														Vector2f(sinD.x, sinD.y));
			node._midPoint = node._sPoint + cosD + sinD;
			node._ePoint = pos;
		}
		else
		{
			/// ��������苗�����Ȃ����Ȃ����ꍇ
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
	_pos = plPos;
	CalTrigonometric(plPos);		/// �O�p�֐��̒l�����߂Ă���
	Rotation();
}

void Octopus::Rotation()
{
	auto mat = MGetRotAxis(vec3, rad);
	auto dVec1 = Vector2f(mat.m[0][0], mat.m[0][1]).V_Cast();
	auto dVec2 = Vector2f(mat.m[1][0], mat.m[1][1]).V_Cast();
	auto dVec3 = VGet(0,0,-1.0f);
	mat = MGetAxis1(dVec1, dVec2, dVec3, Vector2f(LpGame.GetScreenSize().x / 2,LpGame.GetScreenSize().y / 2).V_Cast());

	node._sPoint = VTransform(node._sPoint.V_Cast(), mat);
	node._midPoint = VTransform(node._midPoint.V_Cast(), mat);
	node._ePoint = VTransform(node._ePoint.V_Cast(), mat);
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::DebugDraw()
{
	// �]���藝���g���Ă̊֐߈ړ�
	DrawLine(node._midPoint.x, node._midPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);		/// �I�_���璆�ԓ_
	DrawLine(node._sPoint.x, node._sPoint.y, node._midPoint.x, node._midPoint.y, 0x00ff00);			/// ���ԓ_����n�_

	DrawCircle(node._sPoint.x, node._sPoint.y, 5, 0xff0000, true);
	DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
	DrawCircle(node._midPoint.x, node._midPoint.y, 5, 0xffff00, true);
}
