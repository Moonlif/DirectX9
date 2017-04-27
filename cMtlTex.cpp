#include "stdafx.h"
#include "cMtlTex.h"


cMtlTex::cMtlTex()
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}


cMtlTex::~cMtlTex()
{
}

void cMtlTex::Render()
{
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, g_pTextureManager->FindTexture(m_sTextureName));
}
