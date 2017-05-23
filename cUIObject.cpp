#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_pParent(NULL)
	, m_isHidden(false)
{
	m_pUI = NULL;
}


cUIObject::~cUIObject()
{
	Destroy();
}

void cUIObject::SetPosition(float x, float y, float z)
{
	m_vPosition = D3DXVECTOR3(x, y, z);
}

void cUIObject::AddChild(cUIObject * pChild)
{
	m_vecChild.push_back(pChild);
}

void cUIObject::Update()
{
	D3DXVECTOR3 vPosition(0, 0, 0);
	if (m_pParent)
	{
		vPosition = m_pParent->GetPosition();
	}
	vPosition = m_vPosition + vPosition;

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, vPosition.x, vPosition.y, vPosition.z);

	m_matWorld = matT;

	for each (auto c in m_vecChild)
	{
		c->Update();
	}
}

void cUIObject::Render(LPD3DXSPRITE pSprite)
{
	for each (auto c in m_vecChild)
		c->Render(pSprite);
}

void cUIObject::Destroy()
{
	for each (auto c in m_vecChild)
		c->Destroy();

	this->Release();
}
