#pragma once

class cCamera;

class cGrid;
class cPyramid;
class cCubePC;
class cCubeMan;

class cGroup;
class iMap;
class cFrame;

class cWoman;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCubePC*		m_pCubePC;
	cCubeMan*		m_pCubeMan;
	cCubeMan*		m_pCubeMan2;
	cCamera*		m_pCamera;
	cGrid*			m_pGrid;
	cPyramid*		m_pPyramid;
	
	vector<cGroup*> m_vecGroup;
	iMap*			m_pMap;
	cFrame*			m_pRootFrame;
	cWoman*			m_pWoman;

	//font
	LPD3DXFONT		m_pFont;
	ID3DXMesh*		m_p3DText;
	int				m_nTick1;
	int				m_nTick2;
	int				m_nTick3;
	int				m_nTick4;
	DWORD			m_dwRenderTime;

	//texture test
	LPDIRECT3DTEXTURE9			m_pTexture;
	std::vector<ST_PT_VERTEX>	m_vecVertex;

public:
	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Light();
	void Obj_Render();
	void Load_Surface();

	//font
	void Create_Font();
	void Text_Render();
};

