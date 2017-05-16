#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
	: m_pMtlTex(NULL)
	, m_pVertexBuffer(NULL)
	, m_nNumTri(0)
{
}


cGroup::~cGroup()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVertexBuffer);
}

void cGroup::Render()
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
	}
	else g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	//g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));
	
	g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);
}

void cGroup::BuildVertexBuffer(std::vector<ST_PNT_VERTEX>& vecVertex)
{
	m_nNumTri = vecVertex.size() / 3;
	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX),
		0,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		NULL
	);

	ST_PNT_VERTEX* pV = NULL;
	m_pVertexBuffer->Lock(
		0,
		0,
		(LPVOID*)&pV,
		0
	);

	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));

	m_pVertexBuffer->Unlock();
}