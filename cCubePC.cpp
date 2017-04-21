#include "stdafx.h"
#include "cCubePC.h"


cCubePC::cCubePC()
	: m_vDirection(0, 0, 1)
	, m_vPosition(0, 0, 0)
	, m_vRotation(0, 0, 0)
	, m_vScale(1.0f, 1.0f, 1.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCubePC::~cCubePC()
{
}

void cCubePC::Setup()
{
	ST_PC_VERTEX v;
	v.c = D3DCOLOR_XRGB(128, 0, 0);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 128, 0);
	v.p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 0, 128);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(128, 128, 0);
	v.p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(128, 0, 128);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 128, 128);
	v.p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
							
	v.p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);	m_vecVertex.push_back(v);
}

void cCubePC::Update()
{
	//크기 조절 (Q, E)
	if (GetKeyState('Q') & 0x8000)
	{
		if (m_vScale.x < 2.0f)
			m_vScale = m_vScale + D3DXVECTOR3(0.05f, 0.05f, 0.05f);
	}
	else if (GetKeyState('E') & 0x8000)
	{
		if (m_vScale.x > 0.2f)
			m_vScale = m_vScale - D3DXVECTOR3(0.05f, 0.05f, 0.05f);
	}

	//박스 박향 회전 (A,D)
	if (GetKeyState('A') & 0x8000)
	{
		m_vRotation.y -= 0.02f / m_vScale.x;
	}
	else if (GetKeyState('D') & 0x8000)
	{
		m_vRotation.y += 0.02f / m_vScale.x;
	}

	//박스 이동 (W,S)
	if (GetKeyState('W') & 0x8000)
	{
		m_vPosition = m_vPosition + m_vDirection * 0.05f / m_vScale.x;
		m_vRotation.x += 0.05f / m_vScale.x;
	}
	else if (GetKeyState('S') & 0x8000)
	{
		m_vPosition = m_vPosition - m_vDirection * 0.05f / m_vScale.x;
		m_vRotation.x -= 0.05f / m_vScale.x;
	}

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXMATRIXA16 matR, matRX, matRY, matRZ;
	D3DXMatrixRotationX(&matRX, m_vRotation.x);
	D3DXMatrixRotationY(&matRY, m_vRotation.y);
	D3DXMatrixRotationZ(&matRZ, m_vRotation.z);
	matR = matRX * matRY * matRZ;

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld = matS * matR * matT;

	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRY);
}

void cCubePC::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PC_VERTEX));
}

D3DXVECTOR3 & cCubePC::GetPosition()
{
	return m_vPosition;
}
