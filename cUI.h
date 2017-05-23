#pragma once
class cUIObject;
class cUI
{
public:
	cUI();
	~cUI();

private:
	cUIObject*		m_pUIQuest;
	LPD3DXSPRITE	m_pSpriteQuest;
	LPD3DXFONT		m_pFont;
	RECT			m_stRect;

	RECT			m_stRectTitle;
	bool			m_isClickedTitle;
	POINT			m_ptClicked;
	std::string		m_sText;

public:
	void Setup();
	void Update();
	void Render();

	bool m_isMouseOnUI;
	bool m_isHidden;
	void CloseUI();
	void SetText1();
	void SetText2();
};

