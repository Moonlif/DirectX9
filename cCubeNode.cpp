#include "stdafx.h"
#include "cCubeNode.h"


cCubeNode::cCubeNode()
	: m_vLocalPos(0, 0, 0)
	, m_pParentWorldTM(NULL)
	, m_fRotDeltaX(0.0f)
	, m_fRotDeltaZ(0.0f)
	, m_IsMoving(NULL)
	, m_IsJumping(NULL)
	, m_Isfront(false)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cCubeNode::~cCubeNode()
{
}

void cCubeNode::AddChild(cCubeNode * pChild)
{
	pChild->m_pParentWorldTM = &m_matWorldTM;
	pChild->m_IsMoving = m_IsMoving;
	pChild->m_IsJumping = m_IsJumping;

	m_vecChild.push_back(pChild);
}

void cCubeNode::Destroy()
{
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Destroy();
	}
	delete this;

	//teacher code
	/*for each (auto p in m_vecChild)
	{
		p->Destroy();
	}
	delete this;
	*/
}

void cCubeNode::Setup()
{
}

void cCubeNode::Update()
{
	//local rotation
	D3DXMATRIXA16 matR, matRX, matRZ;
	D3DXMatrixRotationX(&matRX, m_fRotDeltaX);
	D3DXMatrixRotationZ(&matRZ, m_fRotDeltaZ);
	matR = matRX * matRZ;

	//local translation
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	//local matrix
	m_matLocalTM = matR * matT;

	//world matrix
	D3DXMATRIXA16 matParent;
	D3DXMatrixIdentity(&matParent);
	if (m_pParentWorldTM) matParent = *m_pParentWorldTM;
	m_matWorldTM = m_matLocalTM * matParent;

	//set child
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Update();
	}

	//teacher code
	/*for each (auto p in m_vecChild)
	{
		p->Update();
	}*/
}

void cCubeNode::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);

	cCubePNT::Render();

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}

	//teacher code
	/*for each (auto p in m_vecChild)
	{
		p->Render();
	}*/
}

void cCubeNode::Swing()
{
	if (*GetIsJumping() == true)
	{
		if (m_Isfront == true) SetRotDeltaX(D3DX_PI / 3.0f);
		else if (m_Isfront == false) SetRotDeltaX(-D3DX_PI / 3.0f);
	}
	else
	{
		if (*GetIsMoving() == true)
		{
			if (m_Isfront == true)
			{
				if (GetRotDeltaX() < D3DX_PI / 3.0f) SetRotDeltaX(GetRotDeltaX() + 0.15f);
				else m_Isfront = false;
			}
			else if (m_Isfront == false)
			{
				if (GetRotDeltaX() > -D3DX_PI / 3.0f) SetRotDeltaX(GetRotDeltaX() - 0.15f);
				else m_Isfront = true;
			}			
		}
		else
		{
			SetRotDeltaX(0.0f);
		}
	}
}
