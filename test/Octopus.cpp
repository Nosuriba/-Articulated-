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
		/// IK�p�̊֐߂�ǉ����Ă���
		testNode.push_back(Test(Vector2f(300 + (distance * i), 400), 90));
	}
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
void Octopus::CalRadian(const Vector2f & pos)
{
	auto d = pos - testNode[0]._pos;
	auto tan2 = atan2(d.y, d.x);

	testNode[0]._rad = tan2 * 180.0 / DX_PI;
}

//// �֐߂̈ړ�(IK�̃e�X�g�p�֐�)
/// @param tNode �֐�
/// @param vRad  �֐߂̉�]�p�x
/// @param lRad ��]�p�x�̌��E�l
/// @param distance �֐ߊԂ̋���
/// @param pos �ڕW���W
void Octopus::TestMove(std::vector<Test> tNode, float vRad, float lRad, float d, Vector2f pos)
{
	float cosT = cos(vRad);
	float sinT = sin(vRad);

	/// ��]�p�x�̌v�Z
	Test nTip = testNode[testNode.size() - 1];		/// ��[�̎w��
	for (int i = testNode.size() - 1; i >= 0; --i)
	{
		Test n = testNode[i];
		
		/// �֐߂��玩�@�ւ��޸��
		float dx = pos.x - n._pos.x;
		float dy = pos.y - n._pos.y;

		/// �֐߂����[�޸�قւ̓��ς̌v�Z
		TestVec jNode;
		jNode._vec.x = nTip._pos.x - n._pos.x;
		jNode._vec.y = nTip._pos.y - n._pos.y;
		jNode.Dot(Vector2f(dx, dy));

		/// �E��]�޸�ق̌v�Z
		TestVec right;
		if (n._rad + vRad <= lRad)
		{
			right._vec.x = cosT * jNode._vec.x - sinT * jNode._vec.y;
			right._vec.y = sinT * jNode._vec.x + cosT * jNode._vec.y;
			right.Dot(Vector2f(dx, dy));
		}
		else
		{
			/// ��]���ł��Ȃ���
			right.dot = jNode.dot;
		}

		/// ����]���޸�ق̌v�Z
		TestVec left;
		if (n._rad - vRad >= -lRad)
		{
			left._vec.x = cosT * jNode._vec.x + sinT * jNode._vec.y;
			left._vec.y = -sinT * jNode._vec.y + cosT * jNode._vec.y;
			left.Dot(Vector2f(dx, dy));
		}
		else
		{
			/// ��]���ł��Ȃ���
			left.dot = jNode.dot;
		}

		/// ��]�����̑I��
		if (right.dot > jNode.dot && right.dot > left.dot)
		{
			/// �E��]
			n._rad += vRad;
			nTip._pos.x = n._pos.x + right._vec.x;
			nTip._pos.y = n._pos.y + right._vec.y;
		}

		if (left.dot > jNode.dot && left.dot > right.dot)
		{
			/// ����]
			n._rad -= vRad;
			nTip._pos.x = n._pos.x + left._vec.x;
			nTip._pos.y = n._pos.y + left._vec.y;
		}
	}

	/// ���W�̌v�Z
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
	//waitCnt--;					/// ��Ŏg������	
	CalTrigonometric(plPos);		/// �O�p�֐��̒l�����߂Ă���
	CalRadian(plPos);
	TestMove(testNode, testNode[0]._rad, 180, distance, plPos);
}

void Octopus::Draw()
{
	DebugDraw();
}

void Octopus::DebugDraw()
{
	/// �]���藝���g���Ă̊֐߈ړ�
	/*DrawLine(node._midPoint.x, node._midPoint.y, node._ePoint.x, node._ePoint.y, 0x00ccff);		/// �I�_���璆�ԓ_
	DrawLine(node._sPoint.x, node._sPoint.y, node._midPoint.x, node._midPoint.y, 0x00ff00);			/// ���ԓ_����n�_

	DrawCircle(node._sPoint.x, node._sPoint.y, 5, 0xff0000, true);
	DrawCircle(node._ePoint.x, node._ePoint.y, 5, 0xff8800, true);
	DrawCircle(node._midPoint.x, node._midPoint.y, 5, 0xffff00, true);

	DrawFormatString(0, 0, 0xffffff, "cos�̒l : %f", node.cos);
	DrawFormatString(0, 30, 0xffffff, "sin�̒l : %f", node.sin);*/

	/// IK���g���Ă̊֐߈ړ�
	for (int i = 1; i < testNode.size(); ++i)
	{
		auto sPos = testNode[i - 1]._pos;
		auto ePos = testNode[i]._pos;
		DrawLine(sPos.x, sPos.y, ePos.x, ePos.y, 0xffff00);
		DrawCircle(sPos.x, sPos.y, 5, 0xffffff, true);			/// �n�_�̉~
		DrawCircle(ePos.x, ePos.y, 5, 0xffffff, true);			/// �I�_�̉~

	}
}
