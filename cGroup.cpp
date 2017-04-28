#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
	: m_pMtlTex(NULL)
{
}


cGroup::~cGroup()
{
	SAFE_RELEASE(m_pMtlTex);
}

void cGroup::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
	}

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));

	g_pD3DDevice->SetTexture(0, NULL);
}
