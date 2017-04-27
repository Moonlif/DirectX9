#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
{
	m_sObjName.clear();
	m_vecVertex.clear();
	m_vScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


cObject::~cObject()
{
}

void cObject::Setup(string name, D3DMATERIAL9 material, string textureName, vector<ST_PNT_VERTEX> vertex, D3DXVECTOR3 scale)
{
	m_sObjName = name;
	m_stMtl = material;
	m_sTextureName = textureName;
	m_vecVertex = vertex;
	m_vScale = scale;

	D3DXMATRIXA16 matS, matR, mat;
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);

	mat = matS * matR;

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &mat);
	}
}

void cObject::Render()
{
	cMtlTex::Render();

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));

	g_pD3DDevice->SetTexture(0, NULL);
}

void cObject::Destroy()
{
	m_vecVertex.clear();
	delete this;
}
