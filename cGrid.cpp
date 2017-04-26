#include "stdafx.h"
#include "cGrid.h"


cGrid::cGrid()
{
}


cGrid::~cGrid()
{
}

void cGrid::Setup(int xAxisCount, int zAxisCount, float interval)
{
	ST_PC_VERTEX v;
	v.c = D3DCOLOR_XRGB(255, 255, 255);

	//octagon
	v.p = D3DXVECTOR3(5, 0, 0);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(2.5f, 0, 2.5*1.7f); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(2.5f, 0, 2.5*1.7f); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-2.5f, 0, 2.5*1.7f); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-2.5f, 0, 2.5*1.7f); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-5, 0, 0); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-5, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-2.5f, 0, -2.5*1.7f); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-2.5f, 0, -2.5*1.7f); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(2.5f, 0, -2.5*1.7f); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(2.5f, 0, -2.5*1.7f); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(5, 0, 0);	m_vecVertex.push_back(v);


	////x축
	//for (int i = 1; i <= xAxisCount / 2; ++i)
	//{
	//	v.p = D3DXVECTOR3((zAxisCount / 2) * interval, 0, i * interval); m_vecVertex.push_back(v);
	//	v.p = D3DXVECTOR3(-(zAxisCount / 2) * interval, 0, i * interval); m_vecVertex.push_back(v);

	//	v.p = D3DXVECTOR3((zAxisCount / 2) * interval, 0, -i * interval); m_vecVertex.push_back(v);
	//	v.p = D3DXVECTOR3(-(zAxisCount / 2) * interval, 0, -i * interval); m_vecVertex.push_back(v);
	//}
	////z축
	//for (int i = 1; i <= xAxisCount / 2; ++i)
	//{
	//	v.p = D3DXVECTOR3(i * interval, 0, (xAxisCount / 2) * interval); m_vecVertex.push_back(v);
	//	v.p = D3DXVECTOR3(i * interval, 0, -(xAxisCount / 2) * interval); m_vecVertex.push_back(v);

	//	v.p = D3DXVECTOR3(-i * interval, 0, (xAxisCount / 2) * interval); m_vecVertex.push_back(v);
	//	v.p = D3DXVECTOR3(-i * interval, 0, -(xAxisCount / 2) * interval); m_vecVertex.push_back(v);
	//}

	//x축
	v.c = D3DCOLOR_XRGB(255, 0, 0);
	v.p = D3DXVECTOR3(-zAxisCount / 2 - 1.0f, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(zAxisCount / 2 + 1.0f, 0, 0); m_vecVertex.push_back(v);
	//y축
	v.c = D3DCOLOR_XRGB(0, 255, 0);
	v.p = D3DXVECTOR3(0, -zAxisCount / 2 - 1.0f, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, zAxisCount / 2 + 1.0f, 0); m_vecVertex.push_back(v);
	//z축
	v.c = D3DCOLOR_XRGB(0, 0, 255);
	v.p = D3DXVECTOR3(0, 0, -xAxisCount / 2 - 1.0f); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, 0, xAxisCount / 2 + 1.0f); m_vecVertex.push_back(v);
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecVertex.size() / 2, &m_vecVertex[0], sizeof(ST_PC_VERTEX));
}
