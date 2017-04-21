#include "stdafx.h"
#include "cHead.h"


cHead::cHead()
{
}


cHead::~cHead()
{
}

void cHead::Setup()
{
	cCubePNT::Setup();

	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, 0.25f, 0.25f, 0.25f);
	D3DXMatrixTranslation(&matT, 0.0f, 0.1f, 0.0f);
	mat = matS * matT;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}

	m_vLocalPos.x = 0;
	m_vLocalPos.y = 1.025f;
	m_vLocalPos.z = 0;

	m_Isfront = false;
}

void cHead::Update()
{
	if (*GetIsMoving() == false && *GetIsJumping() == false)
	{
		if (m_Isfront == true)
		{
			if (GetRotDeltaX() < D3DX_PI / 5.0f) SetRotDeltaX(GetRotDeltaX() + 0.01f);
			else m_Isfront = false;
		}
		else if (m_Isfront == false)
		{
			if (GetRotDeltaX() > -D3DX_PI / 5.0f) SetRotDeltaX(GetRotDeltaX() - 0.01f);
			else m_Isfront = true;
		}
	}
	else
	{
		SetRotDeltaX(0.0f);
	}

	cCubeNode::Update();
}

