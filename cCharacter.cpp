#include "stdafx.h"
#include "cCharacter.h"
#define GRAVITY_ACCEL 0.01f

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

void cCharacter::Update()
{
	m_IsMoving = false;

	if (m_jumpTime > 0)
	{
		m_jumpTime--;
		m_vPosition.y += 0.3f;
	}

	m_fGravity += GRAVITY_ACCEL;
	m_vPosition.y -= m_fGravity;
	if (m_vPosition.y < 0) m_vPosition.y = 0.0f;

	//jump key (spcae bar)
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (m_IsJumping == false) m_jumpTime = 10;
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
	if (GetKeyState('W') & 0x8000)
	{
		m_vPosition = m_vPosition + m_vDirection * 0.1f;
		m_IsMoving = true;
	}
	else if (GetKeyState('S') & 0x8000)
	{
		m_vPosition = m_vPosition - m_vDirection * 0.1f;
		m_IsMoving = true;
	}

	D3DXMATRIXA16 matR, matRX, matRY, matRZ;
	D3DXMatrixRotationX(&matRX, m_vRotation.x);
	D3DXMatrixRotationY(&matRY, m_vRotation.y);
	D3DXMatrixRotationZ(&matRZ, m_vRotation.z);

	matR = matRX * matRY * matRZ;

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;

	//
	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRY);

	if (m_vPosition.y > 0) m_IsJumping = true;
	else
	{
		m_IsJumping = false;
		m_fGravity = 0.0f;
	}
}

void cCharacter::Render()
{
}

D3DXVECTOR3 & cCharacter::GetPosition()
{
	return m_vPosition;
}
