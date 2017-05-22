#include "stdafx.h"
#include "cWoman.h"
#include "cAseLoader.h"

#include "cRay.h"

cWoman::cWoman()
	: m_pRootStand(NULL)
	, m_pRootRun(NULL)
	, m_IsDestination(false)
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

void cWoman::Update(iMap * pMap, std::vector<ST_PNT_VERTEX>& vecVertexPlane)
{
	//rClick moving
	//if (GetKeyState(VK_RBUTTON) & 0x8000)
	//{
	//	cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
	//	for (int i = 0; i < vecVertexPlane.size(); i += 3)
	//	{
	//		D3DXVECTOR3 v(0, 0, 0);
	//		if (ray.IntersectTri(vecVertexPlane[i + 0].p, vecVertexPlane[i + 1].p, vecVertexPlane[i + 2].p, v))
	//		{
	//			m_IsDestination = true;
	//			m_vDestination = v;
	//		}
	//	}
	//}
	//D3DXVECTOR3 vDistance = m_vDestination - m_vPosition;
	//if (D3DXVec3Length(&vDistance) < 0.1f) m_IsDestination = false;

	//
	if (m_IsDestination == true)
	{
		m_IsMoving = true;

		m_vDirection = m_vDestination - m_vPosition;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);

		D3DXVECTOR3 vPosition = m_vPosition;
		//move
		vPosition = vPosition + m_vDirection * 0.1f;

		//check arrive
		D3DXVECTOR3 vDistance;
		vDistance = m_vDestination - m_vPosition;
		if (D3DXVec3Length(&vDistance) < 0.1f) m_IsDestination = false;
		
		//rotation
		D3DXVECTOR3 zAxis(0, 0, 1);
		m_vRotation.y = acosf(D3DXVec3Dot(&m_vDirection, &zAxis));
		if (m_vDirection.x >= 0) m_vRotation.y += D3DX_PI;
		else if (m_vDirection.x < 0) m_vRotation.y = -m_vRotation.y + D3DX_PI;

		D3DXMATRIXA16 matR, matRX, matRY, matRZ;
		D3DXMatrixRotationX(&matRX, m_vRotation.x);
		D3DXMatrixRotationY(&matRY, m_vRotation.y);
		D3DXMatrixRotationZ(&matRZ, m_vRotation.z);
		matR = matRX * matRY * matRZ;

		//check collision
		if (pMap) pMap->GetHeight(vPosition.x, vPosition.y, vPosition.z);
		m_vPosition = vPosition;

		D3DXMATRIXA16 matT;
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

		m_matWorld = matR * matT;
	}
	else cCharacter::Update(pMap);

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