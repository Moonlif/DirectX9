#include "stdafx.h"
#include "cCharacter.h"

#define GRAVITY_ACCEL 0.01f
#define EPSILON 0.1f

cCharacter::cCharacter()
	: m_vDirection(0, 0, 1)
	, m_vPosition(0, 0, 0)
	, m_vRotation(0, 0, 0)
	, m_IsMoving(false)
	, m_IsJumping(false)
	, m_jumpTime(0)
	, m_fGravity(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCharacter::~cCharacter()
{
}

void cCharacter::Setup()
{
}

void cCharacter::Update(iMap* pMap)
{
	m_pMap = pMap;

	m_IsMoving = false;

	//jump code
	{
		//if (m_jumpTime > 0)
		//{
		//	m_jumpTime--;
		//	m_vPosition.y += 0.2f;
		//}

		//m_fGravity += GRAVITY_ACCEL;
		//if (m_fGravity > 0.50f) m_fGravity = 0.50f;

		//m_vPosition.y -= m_fGravity;

		////jump key (spcae bar)
		//if (GetKeyState(VK_SPACE) & 0x8000)
		//{
		//	if (m_IsJumping == false) m_jumpTime = 30;
		//	m_IsJumping = true;
		//}
	}

	//box direction rotation (A,D)
	if (GetKeyState('A') & 0x8000)
	{
		m_vRotation.y -= 0.1f;
	}
	else if (GetKeyState('D') & 0x8000)
	{
		m_vRotation.y += 0.1f;
	}

	//box move (W,S)
	D3DXVECTOR3 vPosition = m_vPosition;
	if (GetKeyState('W') & 0x8000)
	{
		vPosition = m_vPosition + m_vDirection * 0.05f;

		//if (WallIntersect(true) < 0.2f) m_vPosition = m_vPosition - m_vDirection * 0.1f;

		m_IsMoving = true;
	}
	else if (GetKeyState('S') & 0x8000)
	{
		vPosition = m_vPosition - m_vDirection * 0.05f;

		//if (WallIntersect(false) < 0.2f) m_vPosition = m_vPosition + m_vDirection * 0.1f;

		m_IsMoving = true;
	}

	D3DXMATRIXA16 matR, matRX, matRY, matRZ;
	D3DXMatrixRotationX(&matRX, m_vRotation.x);
	D3DXMatrixRotationY(&matRY, m_vRotation.y);
	D3DXMatrixRotationZ(&matRZ, m_vRotation.z);
	matR = matRX * matRY * matRZ;

	if (pMap)
	{
		if (pMap->GetHeight(vPosition.x, vPosition.y, vPosition.z))
		{
			m_vPosition = vPosition;
		}
	}
	else m_vPosition = vPosition;

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;

	m_vDirection = D3DXVECTOR3(0, 0, -1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRY);
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	//position
	//if (m_vPosition.y - FloorIntersect() > EPSILON) m_IsJumping = true;
	//else
	//{
	//	m_vPosition.y = FloorIntersect();
	//	m_IsJumping = false;
	//	m_fGravity = 0.0f;
	//}
}

void cCharacter::Render()
{
}

D3DXVECTOR3 & cCharacter::GetPosition()
{
	return m_vPosition;
}

//float cCharacter::WallIntersect(bool isFront)
//{
//	int front;
//	if (isFront == true) front = 1;
//	else front = -1;
//
//	float distance = 1000.0f;
//
//	for each(auto it in m_vecGroup2)
//	{
//		for (int i = 0; i < it->GetVertex().size(); i += 3)
//		{
//			float u, v, dis;
//			u = 0.0f;
//			v = 0.0f;
//			dis = 0.0f;
//			D3DXVECTOR3 rayPos = m_vPosition;
//			rayPos.y += 0.2f;
//			D3DXVECTOR3 rayDir = front * m_vDirection;
//
//			D3DXVECTOR3 p1 = it->GetVertex()[i + 0].p;
//			D3DXVECTOR3 p2 = it->GetVertex()[i + 1].p;
//			D3DXVECTOR3 p3 = it->GetVertex()[i + 2].p;
//
//			D3DXMATRIXA16 matWorld, matS, matR;
//			D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
//			D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
//			matWorld = matS * matR;
//			D3DXVec3TransformCoord(&p1, &p1, &matWorld);
//			D3DXVec3TransformCoord(&p2, &p2, &matWorld);
//			D3DXVec3TransformCoord(&p3, &p3, &matWorld);
//
//			if (D3DXIntersectTri(&p1, &p2, &p3, &rayPos, &rayDir, &u, &v, &dis))
//			{
//				if (distance > dis) distance = dis;
//			}
//		}
//	}
//
//	return distance;
//}
