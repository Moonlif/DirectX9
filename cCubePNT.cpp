#include "stdafx.h"
#include "cCubePNT.h"


cCubePNT::cCubePNT()
{
}


cCubePNT::~cCubePNT()
{
}

void cCubePNT::Setup()
{
	//teacher code
	/*
	std::vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.resize(8);

	vecVertex[0].p.x = 0.5f; vecVertex[0].p.y = 0.5f; vecVertex[0].p.z = 0.5f;
	vecVertex[1].p.x = -0.5f; vecVertex[1].p.y = 0.5f; vecVertex[1].p.z = -0.5f;
	vecVertex[2].p.x = 0.5f; vecVertex[2].p.y = 0.5f; vecVertex[2].p.z = -0.5f;
	vecVertex[3].p.x = 0.5f; vecVertex[3].p.y = -0.5f; vecVertex[3].p.z = -0.5f;
	vecVertex[4].p.x = -0.5f; vecVertex[4].p.y = -0.5f; vecVertex[4].p.z = 0.5f;
	vecVertex[5].p.x = -0.5f; vecVertex[5].p.y = 0.5f; vecVertex[5].p.z = 0.5f;
	vecVertex[6].p.x = 0.5f; vecVertex[6].p.y = 0.5f; vecVertex[6].p.z = 0.5f;
	vecVertex[7].p.x = 0.5f; vecVertex[7].p.y = -0.5f; vecVertex[7].p.z = 0.5f;

	///
	std::vector<DWORD> vecIndex;

	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);

	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);

	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);

	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);

	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);

	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);

	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);

	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);

	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);

	m_vecVertex.resize(36);

	for (size_t i = 0; i < vecIndex.size(); i += 3)
	{
		m_vecVertex[i + 0] = vecVertex[vecIndex[i + 0]];
		m_vecVertex[i + 1] = vecVertex[vecIndex[i + 1]];
		m_vecVertex[i + 2] = vecVertex[vecIndex[i + 2]];
	}
*/

	ST_PNT_VERTEX v;
	//µÞ¸é
	v.p = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	//¿ìÃø¸é
	v.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	//ÁÂÃø¸é
	v.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	//¾Õ¸é
	v.p = D3DXVECTOR3(0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	//À­¸é
	v.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, 0.5f, -0.5f);	m_vecVertex.push_back(v);
	//¹Ø¸é
	v.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);	m_vecVertex.push_back(v);

	D3DXVECTOR3 v1, v2, n;
	for (int i = 0; i < 36; i += 3)
	{
		v1 = m_vecVertex[i + 1].p - m_vecVertex[i].p;
		v2 = m_vecVertex[i + 2].p - m_vecVertex[i].p;

		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		m_vecVertex[i].n = n;
		m_vecVertex[i+1].n = n;
		m_vecVertex[i+2].n = n;
	}

	for (int i = 0; i < 36; i++)
	{
		m_vecVertex[i].t.x = 0;
		m_vecVertex[i].t.y = 0;
	}
}

void cCubePNT::Update()
{
}

void cCubePNT::Render()
{
	if (m_vecVertex.size() > 0)
	{
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));
	}
}