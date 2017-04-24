#include "stdafx.h"
#include "cLArm.h"


cLArm::cLArm()
{
}


cLArm::~cLArm()
{
}

void cLArm::Setup()
{
	cCubePNT::Setup();

	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, 0.15f, 0.4f, 0.15f);
	D3DXMatrixTranslation(&matT, -0.275f, -0.2f, 0.0f);
	mat = matS * matT;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}

	m_vLocalPos.x = 0;
	m_vLocalPos.y = 1.0f;
	m_vLocalPos.z = 0;

	m_Isfront = false;

	//texture
	m_vecVertex[0].t = D3DXVECTOR2(12, 31);
	m_vecVertex[1].t = D3DXVECTOR2(12, 20);
	m_vecVertex[2].t = D3DXVECTOR2(16, 20);
	m_vecVertex[3].t = D3DXVECTOR2(12, 31);
	m_vecVertex[4].t = D3DXVECTOR2(16, 20);
	m_vecVertex[5].t = D3DXVECTOR2(16, 31);

	m_vecVertex[6].t = D3DXVECTOR2(0, 31);
	m_vecVertex[7].t = D3DXVECTOR2(0, 20);
	m_vecVertex[8].t = D3DXVECTOR2(4, 20);
	m_vecVertex[9].t = D3DXVECTOR2(0, 31);
	m_vecVertex[10].t = D3DXVECTOR2(4, 20);
	m_vecVertex[11].t = D3DXVECTOR2(4, 31);

	m_vecVertex[12].t = D3DXVECTOR2(8, 31);
	m_vecVertex[13].t = D3DXVECTOR2(8, 20);
	m_vecVertex[14].t = D3DXVECTOR2(12, 20);
	m_vecVertex[15].t = D3DXVECTOR2(8, 31);
	m_vecVertex[16].t = D3DXVECTOR2(12, 20);
	m_vecVertex[17].t = D3DXVECTOR2(12, 31);

	m_vecVertex[18].t = D3DXVECTOR2(4, 31);
	m_vecVertex[19].t = D3DXVECTOR2(4, 20);
	m_vecVertex[20].t = D3DXVECTOR2(8, 20);
	m_vecVertex[21].t = D3DXVECTOR2(4, 31);
	m_vecVertex[22].t = D3DXVECTOR2(8, 20);
	m_vecVertex[23].t = D3DXVECTOR2(8, 31);

	m_vecVertex[24].t = D3DXVECTOR2(4, 19);
	m_vecVertex[25].t = D3DXVECTOR2(4, 16);
	m_vecVertex[26].t = D3DXVECTOR2(8, 16);
	m_vecVertex[27].t = D3DXVECTOR2(4, 19);
	m_vecVertex[28].t = D3DXVECTOR2(8, 16);
	m_vecVertex[29].t = D3DXVECTOR2(8, 19);

	m_vecVertex[30].t = D3DXVECTOR2(8, 19);
	m_vecVertex[31].t = D3DXVECTOR2(8, 16);
	m_vecVertex[32].t = D3DXVECTOR2(12, 16);
	m_vecVertex[33].t = D3DXVECTOR2(8, 19);
	m_vecVertex[34].t = D3DXVECTOR2(12, 16);
	m_vecVertex[35].t = D3DXVECTOR2(12, 19);

	for (int i = 0; i < 36; ++i)
	{
		m_vecVertex[i].t.x *= 0.015625f;
		m_vecVertex[i].t.y *= 0.03125f;
	}
}

void cLArm::Update()
{
	cCubeNode::Swing();

	cCubeNode::Update();
}
