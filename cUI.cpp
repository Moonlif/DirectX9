#include "stdafx.h"
#include "cUI.h"

#include "cUIObject.h"
#include "cUIImageView.h"
#include "cUITextView.h"

enum eUiTag
{
	E_BUTTON_OK = 11,
	E_BUTTON_CANCEL,
	E_BUTTON_EXIT,
	E_TEXT_VIEW,
};

cUI::cUI()
	: m_pUIRoot(NULL)
	, m_pSprite(NULL)
{
}


cUI::~cUI()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot) m_pUIRoot->Destroy();
}

void cUI::Setup()
{
	cUIImageView* pImageView = new cUIImageView;
	pImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	pImageView->SetTexture("UI/panel-info.png");
	m_pUIRoot = pImageView;

	cUITextView* pTextView = new cUITextView;
	pTextView->SetText("좀비출현");
	pTextView->SetSize(ST_SIZEN(300, 200));
	pTextView->SetPosition(D3DXVECTOR3(100, 100, 0));
	pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
	pTextView->SetTag(E_TEXT_VIEW);
	m_pUIRoot->AddChild(pTextView);

	cUIButton* pButtonOK = new cUIButton;
	pButtonOK->SetTexture("UI/btn-med-up.png", "UI/btn-med-over.png", "UI/btn-med-down.png");
	pButtonOK->SetPosition(D3DXVECTOR3(135, 330, 0));
	pButtonOK->SetDelegate(this);
	pButtonOK->SetTag(E_BUTTON_OK);
	m_pUIRoot->AddChild(pButtonOK);

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
}

void cUI::Update()
{
	if (m_pUIRoot) m_pUIRoot->Update();
}

void cUI::Render()
{
	if (m_pUIRoot) m_pUIRoot->Render(m_pSprite);
}

void cUI::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);
	
	if (pSender->GetTag() == E_BUTTON_OK)
	{
		pTextView->SetText("확인");
	}
	//else if ();
}
