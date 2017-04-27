#include "stdafx.h"
#include "cCubeMan.h"
#include "cCubeNode.h"

#include "cHead.h"
#include "cBody.h"
#include "cLArm.h"
#include "cRArm.h"
#include "cLLeg.h"
#include "cRLeg.h"

#define EPSILON 0.1f

cCubeMan::cCubeMan()
	: m_pRoot(NULL)
	, m_pTexture(NULL)
	, m_IsLerp(false)
	, m_nDestIndex(0)
{
	m_pRoot = new cCubeNode;
}


cCubeMan::~cCubeMan()
{
	if (m_pRoot) m_pRoot->Destroy();
	SAFE_RELEASE(m_pTexture);
}

void cCubeMan::Setup(bool isLerp)
{
	Set_Material();

	m_pRoot->SetParentWorldTM(&m_matWorld);
	m_pRoot->SetIsMoving(&m_IsMoving);
	m_pRoot->SetIsJumping(&m_IsJumping);

	cCubeNode* part;
	part = new cHead;
	part->Setup();
	m_pRoot->AddChild(part);

	part = new cBody;
	part->Setup();
	m_pRoot->AddChild(part);

	part = new cLArm;
	part->Setup();
	m_pRoot->AddChild(part);

	part = new cRArm;
	part->Setup();
	m_pRoot->AddChild(part);

	part = new cLLeg;
	part->Setup();
	m_pRoot->AddChild(part);

	part = new cRLeg;
	part->Setup();
	m_pRoot->AddChild(part);

	//texture
	D3DXCreateTextureFromFile(g_pD3DDevice, "batman.png", &m_pTexture);

	//m_vPosition = D3DXVECTOR3(5, 0, 0);

	//destination setting
	m_vDest[0] = D3DXVECTOR3(5.0f, 0, 0);
	m_vDest[1] = D3DXVECTOR3(2.5f, 0, 4.25f);
	m_vDest[2] = D3DXVECTOR3(-2.5f, 0, 4.25f);
	m_vDest[3] = D3DXVECTOR3(-5.0f, 0, 0);
	m_vDest[4] = D3DXVECTOR3(-2.5f, 0, -4.25f);
	m_vDest[5] = D3DXVECTOR3(2.5f, 0, -4.25f);

	m_IsLerp = isLerp;
}

void cCubeMan::Update()
{
	//MoveToDest();

	cCharacter::Update();
	//m_IsMoving = true;

	if (m_pRoot) m_pRoot->Update();
}

void cCubeMan::Render()
{
	//teacher code
	//material setting
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	//

	g_pD3DDevice->SetTexture(0, m_pTexture);

	if (m_pRoot) m_pRoot->Render();

	g_pD3DDevice->SetTexture(0, NULL);
}

void cCubeMan::Set_Material()
{
	//standard setting
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stMtl.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//m_stMtl.Emissive = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
	m_stMtl.Power = 2.0f;
}

void cCubeMan::MoveToDest()
{
	m_vPosition = m_vPosition + m_vDirection * 0.1f;

	if (m_IsLerp == false)
	{
		int destIdx = (m_nDestIndex + 1) % 6;
		D3DXVECTOR3 destPos = m_vDest[destIdx];

		D3DXVECTOR3 dir = destPos - m_vPosition;
		D3DXVec3Normalize(&dir, &dir);
		D3DXVECTOR3 zAxis(0, 0, 1);
		m_vRotation.y = -acosf(D3DXVec3Dot(&dir, &zAxis));
		if (dir.x > 0) m_vRotation.y *= -1;

		D3DXVECTOR3 distance = destPos - m_vPosition;
		if (D3DXVec3Length(&distance) < EPSILON)
		{
			m_vPosition = destPos;
			m_nDestIndex = destIdx;
		}
	}

	else if (m_IsLerp == true)
	{
		int viaIdx, destIdx;
		viaIdx = (m_nDestIndex + 1) % 6;
		destIdx = (viaIdx + 1) % 6;
		D3DXVECTOR3 destPos = m_vDest[destIdx];
		D3DXVECTOR3 viaPos = m_vDest[viaIdx];

		D3DXVECTOR3 viaToDest = destPos - viaPos;

		//float length = D3DXVec3Length(&viaToDest);
		//int count = 10;
		//float dLength = length / count;
		//m_vSavePos;

		m_vViaPos = m_vViaPos + viaToDest * 0.02f;
		if (D3DXVec3Length(&m_vViaPos) > D3DXVec3Length(&viaToDest))
		{
			m_vViaPos = viaToDest;
		}

		D3DXVECTOR3 target = viaPos + m_vViaPos;

		D3DXVECTOR3 dir = target - m_vPosition;
		D3DXVec3Normalize(&dir, &dir);
		D3DXVECTOR3 zAxis(0, 0, 1);
		m_vRotation.y = -acosf(D3DXVec3Dot(&dir, &zAxis));
		if (dir.x > 0) m_vRotation.y *= -1;

		D3DXVECTOR3 distance = destPos - m_vPosition;
		if (D3DXVec3Length(&distance) < EPSILON)
		{
			m_vPosition = destPos;
			m_nDestIndex = destIdx;
			m_vViaPos = D3DXVECTOR3(0, 0, 0);
		}
	}

	//º£Áö¾î °î¼±

}
