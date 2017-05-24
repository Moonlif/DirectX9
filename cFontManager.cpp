#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	if (m_mapFont.find(e) != m_mapFont.end())
		return m_mapFont[e];

	D3DXFONT_DESC fontDiscription;
	ZeroMemory(&fontDiscription, sizeof(D3DXFONT_DESC));

	if (e == E_DEFAULT)
	{
		fontDiscription.Height = 25;
		fontDiscription.Width = 12;
		fontDiscription.Weight = FW_MEDIUM;
		fontDiscription.Italic = false;
		fontDiscription.CharSet = DEFAULT_CHARSET;
		fontDiscription.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDiscription.PitchAndFamily = FF_DONTCARE;
		strcpy_s(fontDiscription.FaceName, "±¼¸²Ã¼");
	}
	else if (e == E_QUEST)
	{
		fontDiscription.Height = 25;
		fontDiscription.Width = 12;
		fontDiscription.Weight = FW_MEDIUM;
		fontDiscription.Italic = false;
		fontDiscription.CharSet = DEFAULT_CHARSET;
		fontDiscription.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDiscription.PitchAndFamily = FF_DONTCARE;

		AddFontResource("font/umberto.ttf");
		strcpy_s(fontDiscription.FaceName, "umberto");
	}

	D3DXCreateFontIndirect(g_pD3DDevice, &fontDiscription, &m_mapFont[e]);

	return m_mapFont[e];
}

void cFontManager::Destroy()
{
	for each(auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapFont.clear();
}
