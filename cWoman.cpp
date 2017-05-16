#include "stdafx.h"
#include "cWoman.h"
#include "cAseLoader.h"

cWoman::cWoman()
	: m_pRootStand(NULL)
	, m_pRootRun(NULL)
{
}


cWoman::~cWoman()
{
	if (m_pRootStand) m_pRootStand->Destroy();
	if (m_pRootRun) m_pRootRun->Destroy();
}

void cWoman::Setup()
{
	cAseLoader Load1, Load2;
	m_pRootRun = Load1.Load("woman/woman_01_all.ASE");
	m_pRootStand = Load2.Load("woman/woman_01_all_stand.ASE");

	m_vPosition = D3DXVECTOR3(0, 0, 0);
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
}
