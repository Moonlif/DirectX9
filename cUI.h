#pragma once

#include "cUIButton.h"
class cUIObject;

class cUI : public iButtonDelegate
{
public:
	cUI();
	~cUI();

private:
	cUIObject*		m_pUIRoot;
	LPD3DXSPRITE	m_pSprite;

public:
	void Setup();
	void Update();
	void Render();

	virtual void OnClick(cUIButton* pSender) override;
};

