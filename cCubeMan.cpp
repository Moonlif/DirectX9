#include "stdafx.h"
#include "cCubeMan.h"
#include "cCubeNode.h"

#include "cHead.h"
#include "cBody.h"
#include "cLArm.h"
#include "cRArm.h"
#include "cLLeg.h"
#include "cRLeg.h"

cCubeMan::cCubeMan()
	: m_pRoot(NULL)
	, m_pTexture(NULL)
{
	m_pRoot = new cCubeNode;
}


cCubeMan::~cCubeMan()
{
	if (m_pRoot) m_pRoot->Destroy();
	SAFE_RELEASE(m_pTexture);
}

void cCubeMan::Setup()
{
	//teacher code
	//material setting
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	//

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
}

void cCubeMan::Update()
{
	cCharacter::Update();

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
