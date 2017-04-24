#include "stdafx.h"
#include "cBody.h"


cBody::cBody()
{
}


cBody::~cBody()
{
}

void cBody::Setup()
{
	cCubePNT::Setup();

	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, 0.4f, 0.5f, 0.2f);
	D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
	mat = matS * matT;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}

	m_vLocalPos.x = 0;
	m_vLocalPos.y = 0.75f;
	m_vLocalPos.z = 0;

	//texture
	m_vecVertex[0].t = D3DXVECTOR2(32, 31);
	m_vecVertex[1].t = D3DXVECTOR2(32, 20);
	m_vecVertex[2].t = D3DXVECTOR2(39, 20);
	m_vecVertex[3].t = D3DXVECTOR2(32, 31);
	m_vecVertex[4].t = D3DXVECTOR2(39, 20);
	m_vecVertex[5].t = D3DXVECTOR2(39, 31);

	m_vecVertex[6].t = D3DXVECTOR2(16, 31);
	m_vecVertex[7].t = D3DXVECTOR2(16, 20);
	m_vecVertex[8].t = D3DXVECTOR2(19, 20);
	m_vecVertex[9].t = D3DXVECTOR2(16, 31);
	m_vecVertex[10].t = D3DXVECTOR2(19, 20);
	m_vecVertex[11].t = D3DXVECTOR2(19, 31);

	m_vecVertex[12].t = D3DXVECTOR2(28, 31);
	m_vecVertex[13].t = D3DXVECTOR2(28, 20);
	m_vecVertex[14].t = D3DXVECTOR2(35, 20);
	m_vecVertex[15].t = D3DXVECTOR2(28, 31);
	m_vecVertex[16].t = D3DXVECTOR2(35, 20);
	m_vecVertex[17].t = D3DXVECTOR2(35, 31);

	m_vecVertex[18].t = D3DXVECTOR2(20, 31);
	m_vecVertex[19].t = D3DXVECTOR2(20, 20);
	m_vecVertex[20].t = D3DXVECTOR2(27, 20);
	m_vecVertex[21].t = D3DXVECTOR2(20, 31);
	m_vecVertex[22].t = D3DXVECTOR2(27, 20);
	m_vecVertex[23].t = D3DXVECTOR2(27, 31);

	m_vecVertex[24].t = D3DXVECTOR2(20, 19);
	m_vecVertex[25].t = D3DXVECTOR2(20, 16);
	m_vecVertex[26].t = D3DXVECTOR2(27, 16);
	m_vecVertex[27].t = D3DXVECTOR2(20, 19);
	m_vecVertex[28].t = D3DXVECTOR2(27, 16);
	m_vecVertex[29].t = D3DXVECTOR2(27, 19);

	m_vecVertex[30].t = D3DXVECTOR2(28, 19);
	m_vecVertex[31].t = D3DXVECTOR2(28, 16);
	m_vecVertex[32].t = D3DXVECTOR2(35, 16);
	m_vecVertex[33].t = D3DXVECTOR2(28, 19);
	m_vecVertex[34].t = D3DXVECTOR2(35, 16);
	m_vecVertex[35].t = D3DXVECTOR2(35, 19);

	for (int i = 0; i < 36; ++i)
	{
		m_vecVertex[i].t.x *= 0.015625f;
		m_vecVertex[i].t.y *= 0.03125f;
	}
}
