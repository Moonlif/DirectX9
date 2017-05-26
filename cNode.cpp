#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
	: m_fF(0.0f)
	, m_fG(0.0f)
	, m_fH(0.0f)
	, m_pParent(NULL)
	, m_isClosed(false)
	, m_isInOpenList(false)
	, m_nCol(0)
	, m_nRow(0)
	, m_eNodeType(E_NONE)
	, m_nHeapIndex(0)
{
	SetRect(&m_rc, 0, 0, 0, 0);
}


cNode::~cNode()
{
}

void cNode::Setup(int nIndex, eNodeType e)
{
	m_nRow = nIndex / TILE_N;
	m_nCol = nIndex % TILE_N;

	m_eNodeType = e;
	m_rc.left = m_nCol * TILE_SIZE;
	m_rc.top = m_nRow * TILE_SIZE;
	m_rc.right = m_rc.left + TILE_SIZE;
	m_rc.bottom = m_rc.top + TILE_SIZE;
}

void cNode::Render(HDC hdc)
{
	COLORREF bkColor = RGB(255, 255, 255);

	switch (m_eNodeType)
	{
	case cNode::E_NONE:
		break;
	case cNode::E_EMPTY:
		break;
	case cNode::E_WALL:
		bkColor = RGB(80, 80, 80);
		break;
	case cNode::E_START:
		break;
	case cNode::E_DEST:
		break;
	case cNode::E_CLOSE:
		bkColor = RGB(255, 0, 0);
		break;
	case cNode::E_OPEN:
		bkColor = RGB(0, 255, 0);
		break;
	case cNode::E_PATH:
		bkColor = RGB(255, 255, 0);
		break;
	default:
		assert(false);
		break;
	}

	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(bkColor);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, m_rc.left, m_rc.top, m_rc.right, m_rc.bottom);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	if (m_fF > 0.0f)
	{
		SetBkMode(hdc, TRANSPARENT);
		char szBuf[16];

		sprintf_s(szBuf, "%.1f", m_fG);
		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_LEFT | DT_TOP);

		sprintf_s(szBuf, "%.1f", m_fH);
		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_RIGHT | DT_TOP);

		sprintf_s(szBuf, "%.1f", m_fF);
		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);

		if (m_pParent)
		{
			POINT ptMyCenter;
			ptMyCenter.x = (m_rc.left + m_rc.right) / 2;
			ptMyCenter.y = (m_rc.top + m_rc.bottom) / 2;

			POINT ptParentCenter;
			ptParentCenter.x = (m_pParent->m_rc.left + m_pParent->m_rc.right) / 2;
			ptParentCenter.y = (m_pParent->m_rc.top + m_pParent->m_rc.bottom) / 2;

			POINT ptLineEnd;
			ptLineEnd.x = (ptMyCenter.x + ptParentCenter.x) / 2;
			ptLineEnd.y = (ptMyCenter.y + ptParentCenter.y) / 2;

			MoveToEx(hdc, ptMyCenter.x, ptMyCenter.y, NULL);
			LineTo(hdc, ptLineEnd.x, ptLineEnd.y);

			hBrush = CreateSolidBrush(bkColor);
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Ellipse(hdc, ptMyCenter.x - 3, ptMyCenter.y - 3, ptMyCenter.x + 3, ptMyCenter.y + 3);

			SelectObject(hdc, hOldBrush);
			DeleteObject(hBrush);
		}
	}
}
