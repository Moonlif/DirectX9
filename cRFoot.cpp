#include "stdafx.h"
#include "cRFoot.h"


cRFoot::cRFoot()
{
}


cRFoot::~cRFoot()
{
}

void cRFoot::Setup()
{
	cCubePNT::Setup();

	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, 0.15f, 0.10f, 0.25f);
	D3DXMatrixTranslation(&matT, 0.1f, 0.0f, 0.05f);
	mat = matS * matT;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}

	m_vLocalPos.x = 0;
	m_vLocalPos.y = -0.5f + 0.1f;
	m_vLocalPos.z = 0;

	//texture
	m_vecVertex[0].t = D3DXVECTOR2(52, 31);
	m_vecVertex[1].t = D3DXVECTOR2(52, 27);
	m_vecVertex[2].t = D3DXVECTOR2(56, 27);
	m_vecVertex[3].t = D3DXVECTOR2(52, 31);
	m_vecVertex[4].t = D3DXVECTOR2(56, 27);
	m_vecVertex[5].t = D3DXVECTOR2(56, 31);

	m_vecVertex[6].t = D3DXVECTOR2(40, 31);
	m_vecVertex[7].t = D3DXVECTOR2(40, 27);
	m_vecVertex[8].t = D3DXVECTOR2(44, 27);
	m_vecVertex[9].t = D3DXVECTOR2(40, 31);
	m_vecVertex[10].t = D3DXVECTOR2(44, 27);
	m_vecVertex[11].t = D3DXVECTOR2(44, 31);

	m_vecVertex[12].t = D3DXVECTOR2(48, 31);
	m_vecVertex[13].t = D3DXVECTOR2(48, 27);
	m_vecVertex[14].t = D3DXVECTOR2(52, 27);
	m_vecVertex[15].t = D3DXVECTOR2(48, 31);
	m_vecVertex[16].t = D3DXVECTOR2(52, 27);
	m_vecVertex[17].t = D3DXVECTOR2(52, 31);

	m_vecVertex[18].t = D3DXVECTOR2(44, 31);
	m_vecVertex[19].t = D3DXVECTOR2(44, 27);
	m_vecVertex[20].t = D3DXVECTOR2(48, 27);
	m_vecVertex[21].t = D3DXVECTOR2(44, 31);
	m_vecVertex[22].t = D3DXVECTOR2(48, 27);
	m_vecVertex[23].t = D3DXVECTOR2(48, 31);

	m_vecVertex[24].t = D3DXVECTOR2(44, 31);
	m_vecVertex[25].t = D3DXVECTOR2(44, 27);
	m_vecVertex[26].t = D3DXVECTOR2(48, 27);
	m_vecVertex[27].t = D3DXVECTOR2(44, 31);
	m_vecVertex[28].t = D3DXVECTOR2(48, 27);
	m_vecVertex[29].t = D3DXVECTOR2(48, 31);

	m_vecVertex[30].t = D3DXVECTOR2(48, 19);
	m_vecVertex[31].t = D3DXVECTOR2(48, 16);
	m_vecVertex[32].t = D3DXVECTOR2(52, 16);
	m_vecVertex[33].t = D3DXVECTOR2(48, 19);
	m_vecVertex[34].t = D3DXVECTOR2(52, 16);
	m_vecVertex[35].t = D3DXVECTOR2(52, 19);

	for (int i = 0; i < 36; ++i)
	{
		m_vecVertex[i].t.x *= 0.015625f;
		m_vecVertex[i].t.y *= 0.03125f;
	}
}