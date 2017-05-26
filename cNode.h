#pragma once
#define TILE_N 16
#define TILE_SIZE 50

class cNode
{
public:
	cNode();
	~cNode();

	enum eNodeType
	{
		E_NONE, E_EMPTY, E_WALL, E_START, E_DEST, E_CLOSE, E_OPEN, E_PATH,
	};

	float m_fF;
	float m_fG;
	float m_fH;
	cNode* m_pParent;
	bool m_isClosed;
	bool m_isInOpenList;
	int m_nRow;
	int m_nCol;
	eNodeType m_eNodeType;
	RECT m_rc;
	int m_nHeapIndex;

	void Setup(int nIndex, eNodeType e);
	void Render(HDC hdc);
};

