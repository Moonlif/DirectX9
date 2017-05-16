#include "stdafx.h"
#include "cWoman.h"
#include "cAseLoader.h"

cWoman::cWoman()
	:m_pRootStand(NULL)
	, m_pRootRun(NULL)
	, m_pMesh(NULL)
{
}


cWoman::~cWoman()
{
	if (m_pRootStand) m_pRootStand->Destroy();
	if (m_pRootRun) m_pRootRun->Destroy();

	for each (auto it in m_vecMtlTex)
	{
		SAFE_RELEASE(it);
	}
	SAFE_RELEASE(m_pMesh);
}

void cWoman::Setup()
{
	cAseLoader Load1, Load2;
	m_pRootRun = Load1.Load("woman/woman_01_all.ASE");
	m_pRootStand = Load2.Load("woman/woman_01_all_stand.ASE");

	m_vPosition = D3DXVECTOR3(0, 0, 0);

	cAseLoader Load3;
	m_pMesh = Load3.LoadMesh(m_vecMtlTex, "woman/woman_01_all.ASE");

	std::vector<ST_PNT_VERTEX> vecVertex;
	std::vector<cMtlTex*> vecMtlTex;
	std::vector<DWORD> vecAttribute;
	m_pRootRun->SumMeshInfo(vecVertex, vecMtlTex, vecAttribute);
	for each(auto it in vecMtlTex)
	{
		SAFE_RELEASE(it);
	}
	vecMtlTex.clear();
	m_pRootStand->SumMeshInfo(vecVertex, vecMtlTex, vecAttribute);
	for each(auto it in vecMtlTex)
	{
		SAFE_RELEASE(it);
	}
}

void cWoman::Update(iMap * pMap)
{
	cCharacter::Update(pMap);

	if (m_IsMoving)
	{
		if (m_pRootRun) m_pRootRun->Update(m_pRootRun->GetKeyFrame(), &m_matWorld);
	}
	else
	{
		if (m_pRootStand) m_pRootStand->Update(m_pRootRun->GetKeyFrame(), &m_matWorld);
	}
}

void cWoman::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	if (m_IsMoving)
	{
		if (m_pRootRun)	m_pRootRun->Render();
	}
	else
	{
		if(m_pRootStand) m_pRootStand->Render();
	}

	if (m_pMesh)
	{
		for (int i = 0; i < m_vecMtlTex.size(); ++i)
		{
			D3DXMATRIXA16 matWorld;
			if (m_IsMoving)
			{
				if (m_pRootRun) m_pRootRun->GetWorldTMforAnimation(&matWorld, i);
			}
			else
			{
				if (m_pRootStand) m_pRootStand->GetWorldTMforAnimation(&matWorld, i);
			}
			D3DXMATRIXA16 matT;
			D3DXMatrixTranslation(&matT, 1.0f, 0, 0);
			matWorld = matWorld * matT;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pD3DDevice->SetTexture(0, m_vecMtlTex[i]->GetTexture());
			g_pD3DDevice->SetMaterial(&m_vecMtlTex[i]->GetMaterial());
			g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

			m_pMesh->DrawSubset(i);
		}
	}
}
