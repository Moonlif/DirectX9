#include "stdafx.h"
#include "cWoman.h"
#include "cAseLoader.h"
#include "cRawLoader.h"

cWoman::cWoman()
	: m_pRootStand(NULL)
	, m_pRootRun(NULL)
	, m_IsDestination(false)
	, m_pMeshSphere(NULL)
{
}


cWoman::~cWoman()
{
	if (m_pRootStand) m_pRootStand->Destroy();
	if (m_pRootRun) m_pRootRun->Destroy();
	SAFE_RELEASE(m_pMeshSphere);
}

void cWoman::Setup()
{
	cAseLoader Load1, Load2;
	m_pRootRun = Load1.Load("woman/woman_01_all.ASE");
	m_pRootStand = Load2.Load("woman/woman_01_all_stand.ASE");

	m_vPosition = D3DXVECTOR3(0, 0, 0);

	//sphere setup
	D3DXCreateSphere(g_pD3DDevice, 0.5f, 10, 10, &m_pMeshSphere, NULL);
	ZeroMemory(&m_stMtlSphere, sizeof(D3DMATERIAL9));
	m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
}

void cWoman::Update(iMap * pMap)
{
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

	//sphere
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		D3DXMATRIXA16 matWorld, matT;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matT);
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y + 0.5f, m_vPosition.z);
		matWorld = matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtlSphere);
		m_pMeshSphere->DrawSubset(0);

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void cWoman::SetDestination(D3DXVECTOR3 vDestination)
{
	D3DXVECTOR3 vDistance;
	vDistance = vDestination - m_vPosition;
	if (D3DXVec3Length(&vDistance) < 0.1f) return;

	m_IsDestination = true;
	m_vDestination = vDestination;
}

void cWoman::ApplyHeightMap(std::vector<ST_PNT_VERTEX>& vecVertexHeightMap)
{
	if (m_vPosition.x < 0 || m_vPosition.z < 0) return;

	int x = (int)m_vPosition.x;
	int z = (int)m_vPosition.z;
	int surfaceIndex = x + z * 256;

	D3DXVECTOR3 vRayPos(m_vPosition.x, 1000, m_vPosition.z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	for (int i = surfaceIndex * 6; i < surfaceIndex * 6 + 6; i += 3)
	{
		float u, v, f;

		if (D3DXIntersectTri(
			&vecVertexHeightMap[i + 0].p,
			&vecVertexHeightMap[i + 1].p,
			&vecVertexHeightMap[i + 2].p,
			&vRayPos,
			&vRayDir,
			&u,
			&v,
			&f))
		{
			m_vPosition.y = 1000 - f;
			return;
		}
	}
}

