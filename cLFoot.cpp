#include "stdafx.h"
#include "cLFoot.h"


cLFoot::cLFoot()
{
}


cLFoot::~cLFoot()
{
}

void cLFoot::Setup()
{
	cCubePNT::Setup();

	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, 0.15f, 0.10f, 0.25f);
	D3DXMatrixTranslation(&matT, -0.1f, 0.0f, 0.05f);
	mat = matS * matT;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}

	m_vLocalPos.x = 0;
	m_vLocalPos.y = -0.5f + 0.1f;
	m_vLocalPos.z = 0;
}