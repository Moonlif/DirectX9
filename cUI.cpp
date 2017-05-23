#include "stdafx.h"
#include "cUI.h"

#include "cUIObject.h"
#include "cUIButton.h"
#include "cUIImageView.h"

cUI::cUI()
	: m_pUIQuest(NULL)
	, m_pSpriteQuest(NULL)
	, m_pFont(NULL)
	, m_isMouseOnUI(false)
{
}


cUI::~cUI()
{
	SAFE_RELEASE(m_pUIQuest);
	SAFE_RELEASE(m_pSpriteQuest);
	SAFE_RELEASE(m_pFont);
}

void cUI::Setup()
{
	m_pUIQuest = new cUIObject;
	m_pUIQuest->SetPosition(100, 100);

	cUIImageView* pImage = new cUIImageView;
	pImage->SetParent(m_pUIQuest);
	pImage->SetPosition(0, 0);
	pImage->SetTexture("UI/custom/ui_quest.png");
	m_pUIQuest->AddChild(pImage);

	cUIButton* pButtonX = new cUIButton;
	pButtonX->SetParent(m_pUIQuest);
	pButtonX->SetPosition(330, 10);
	pButtonX->SetTexture("UI/custom/ui_btn_x.png", "UI/custom/ui_btn_x.png", "UI/custom/ui_btn_x.png");
	//pButtonX->ButtonAction = CloseUI;
	pButtonX->SetUI(this);
	pButtonX->SetButtonId(0);
	m_pUIQuest->AddChild(pButtonX);

	cUIButton* pButton1 = new cUIButton;
	pButton1->SetParent(m_pUIQuest);
	pButton1->SetPosition(30, 220);
	pButton1->SetTexture("UI/custom/ui_btn_nor.png", "UI/custom/ui_btn_over.png", "UI/custom/ui_btn_sel.png");
	//pButton1->ButtonAction = SetText1;
	pButton1->SetUI(this);
	pButton1->SetButtonId(1);
	m_pUIQuest->AddChild(pButton1);

	cUIButton* pButton2 = new cUIButton;
	pButton2->SetParent(m_pUIQuest);
	pButton2->SetPosition(200, 220);
	pButton2->SetTexture("UI/custom/ui_btn_nor2.png", "UI/custom/ui_btn_over2.png", "UI/custom/ui_btn_sel2.png");
	//pButton2->ButtonAction = SetText2;
	pButton2->SetUI(this);
	pButton2->SetButtonId(2);
	m_pUIQuest->AddChild(pButton2);	

	D3DXCreateSprite(g_pD3DDevice, &m_pSpriteQuest);

	//font
	D3DXFONT_DESC fontDiscription;
	ZeroMemory(&fontDiscription, sizeof(D3DXFONT_DESC));

	fontDiscription.Height = 30;
	fontDiscription.Width = 15;
	fontDiscription.Weight = FW_MEDIUM;
	fontDiscription.Italic = false;
	fontDiscription.CharSet = DEFAULT_CHARSET;
	fontDiscription.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDiscription.PitchAndFamily = FF_DONTCARE;
	AddFontResource("font/umberto.ttf");
	strcpy_s(fontDiscription.FaceName, "umberto");

	D3DXCreateFontIndirect(g_pD3DDevice, &fontDiscription, &m_pFont);

	m_sText = "quest test";
}

void cUI::Update()
{
	//child update
	if (m_pUIQuest) m_pUIQuest->Update();

	//mouse action
	{
		SetRect(&m_stRect, m_pUIQuest->GetPosition().x, m_pUIQuest->GetPosition().y, m_pUIQuest->GetPosition().x + 386, m_pUIQuest->GetPosition().y + 325);
		if (PtInRect(&m_stRect, g_ptMouse))
		{
			m_isMouseOnUI = true;
		}
		else m_isMouseOnUI = false;

		SetRect(&m_stRectTitle, m_pUIQuest->GetPosition().x, m_pUIQuest->GetPosition().y, m_pUIQuest->GetPosition().x + 386, m_pUIQuest->GetPosition().y + 50);
		//클릭 키다운
		if (PtInRect(&m_stRectTitle, g_ptMouse) && m_isClickedTitle == false)
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_ptClicked = g_ptMouse;
				m_isClickedTitle = true;
			}
		}
		//클릭 키업
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001);
		else m_isClickedTitle = false;

		if (m_isClickedTitle)
		{
			POINT pt;
			pt.x = m_pUIQuest->GetPosition().x + g_ptMouse.x - m_ptClicked.x;
			pt.y = m_pUIQuest->GetPosition().y + g_ptMouse.y - m_ptClicked.y;
			m_pUIQuest->SetPosition(pt.x, pt.y);

			m_ptClicked = g_ptMouse;
		}
	}
}

void cUI::Render()
{
	if (m_pUIQuest) m_pUIQuest->Render(m_pSpriteQuest);

	//text render
	RECT rc;
	SetRect(&rc, m_stRect.left + 100, m_stRect.top + 100, 0, 0);
	if (m_pFont) m_pFont->DrawTextA(NULL, m_sText.c_str(), m_sText.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(88, 12, 8));
}

void cUI::CloseUI()
{
	m_isMouseOnUI = false;
	m_isHidden = true;
}

void cUI::SetText1()
{
	m_sText = "NO.1 activation";
}

void cUI::SetText2()
{
	m_sText = "NO.2 activation";
}
