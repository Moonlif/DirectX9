#include "stdafx.h"
#include "cRArm.h"


cRArm::cRArm()
{
}


cRArm::~cRArm()
{
}

void cRArm::Setup()
{
	cCubePNT::Setup();

	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, 0.15f, 0.4f, 0.15f);
	D3DXMatrixTranslation(&matT, 0.275f, -0.2f, 0.0f);
	mat = matS * matT;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}

	m_vLocalPos.x = 0;
	m_vLocalPos.y = 1.0f;
	m_vLocalPos.z = 0;

	m_Isfront = true;
}

void cRArm::Update()
{
	cCubeNode::Swing();

	cCubeNode::Update();
}

