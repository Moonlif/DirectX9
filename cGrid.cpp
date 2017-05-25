#include "stdafx.h"
#include "cGrid.h"


cGrid::cGrid()
	: m_nNumLine(0)
	, m_pVertexBuffer(NULL)
{
}


cGrid::~cGrid()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void cGrid::Setup(int xAxisStart, int zAxisStart, int xAxisCount, int zAxisCount, float interval)
{
	ST_PC_VERTEX v;
	v.c = D3DCOLOR_XRGB(0, 0, 0);

	//x�� ����
	for (int i = 0; i < xAxisCount; ++i)
	{
		v.p = D3DXVECTOR3(xAxisStart, 0.01f, zAxisStart + i);  m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(xAxisStart + xAxisCount * interval, 0.01f, zAxisStart + i); m_vecVertex.push_back(v);
	}
	//z�� ����
	for (int i = 0; i < xAxisCount; ++i)
	{
		v.p = D3DXVECTOR3(xAxisStart + i, 0.01f, zAxisStart);  m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(xAxisStart + i, 0.01f, zAxisStart + zAxisCount * interval); m_vecVertex.push_back(v);
	}

	//x��
	v.c = D3DCOLOR_XRGB(255, 0, 0);
	v.p = D3DXVECTOR3(-zAxisCount / 2 - 1.0f, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(zAxisCount / 2 + 1.0f, 0, 0); m_vecVertex.push_back(v);
	//y��
	v.c = D3DCOLOR_XRGB(0, 255, 0);
	v.p = D3DXVECTOR3(0, -zAxisCount / 2 - 1.0f, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, zAxisCount / 2 + 1.0f, 0); m_vecVertex.push_back(v);
	//z��
	v.c = D3DCOLOR_XRGB(0, 0, 255);
	v.p = D3DXVECTOR3(0, 0, -xAxisCount / 2 - 1.0f); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, 0, xAxisCount / 2 + 1.0f); m_vecVertex.push_back(v);

	//VertexBuffer
	m_nNumLine = m_vecVertex.size() / 2;
	g_pD3DDevice->CreateVertexBuffer(
		m_vecVertex.size() * sizeof(ST_PC_VERTEX),
		0,
		ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		NULL
	);

	ST_PC_VERTEX* pV = NULL;
	m_pVertexBuffer->Lock(
		0,
		0,
		(LPVOID*)&pV,
		0
	);

	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PC_VERTEX));

	m_pVertexBuffer->Unlock();
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecVertex.size() / 2, &m_vecVertex[0], sizeof(ST_PC_VERTEX));

	//g_pD3DDevice->SetStreamSource(
	//	0,
	//	m_pVertexBuffer,
	//	0,
	//	sizeof(ST_PC_VERTEX)
	//);
	//g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nNumLine);
}
