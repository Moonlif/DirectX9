#include "stdafx.h"
#include "cUIButton.h"
#include "cUI.h"

cUIButton::cUIButton()
	: m_eButtonState(E_NORMAL)
{
	for (int i = 0; i < E_STATE_CNT; ++i)
	{
		m_aTexture[i] = NULL;
	}
}


cUIButton::~cUIButton()
{
	for (int i = 0; i < E_STATE_CNT; ++i)
	{
		SAFE_RELEASE(m_aTexture[i]);
	}
}

void cUIButton::SetTexture(char * szNor, char * szOver, char * szSel)
{
	D3DXCreateTextureFromFileEx(g_pD3DDevice, szNor, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &m_stImageInfo[0], NULL, &m_aTexture[0]);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, szOver, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &m_stImageInfo[1], NULL, &m_aTexture[1]);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, szSel, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &m_stImageInfo[2], NULL, &m_aTexture[2]);
}

void cUIButton::Update()
{
	cUIObject::Update();

	D3DXVECTOR3 vPosition(0, 0, 0);
	if (GetParent() != NULL)
	{
		vPosition = GetParent()->GetPosition();
	}
	vPosition = GetPosition() + vPosition;

	RECT collisionRc;
	SetRect(&collisionRc, vPosition.x, vPosition.y, vPosition.x + m_stImageInfo[m_eButtonState].Width, vPosition.y + m_stImageInfo[m_eButtonState].Height);

	//click
	if (PtInRect(&collisionRc, g_ptMouse))
	{
		m_eButtonState = E_MOUSEOVER;
	}
	else
	{
		m_eButtonState = E_NORMAL;
	}
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_eButtonState == E_MOUSEOVER)
		{
			m_eButtonState = E_SELECTED;
			//ButtonAction();
			if (GetButtonId() == 0) GetUI()->CloseUI();
			else if (GetButtonId() == 1) GetUI()->SetText1();
			else if (GetButtonId() == 2) GetUI()->SetText2();
		}
		else
		{
			m_eButtonState = E_NORMAL;
		}
	}
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);

	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo[m_eButtonState].Width, m_stImageInfo[m_eButtonState].Height);

	pSprite->Draw(m_aTexture[m_eButtonState], &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();
}
