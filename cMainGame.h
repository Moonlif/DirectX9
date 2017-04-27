#pragma once

class cCubePC;
class cCamera;
class cGrid;
class cPyramid;

class cCubeMan;
class cObjLoader;

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
	cObjLoader*		m_pObjLoader;

	D3DXVECTOR3		m_vCamTarget;
	
	//texture test
	LPDIRECT3DTEXTURE9 m_pTexture;
	std::vector<ST_PT_VERTEX> m_vecVertex;

public:
	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Light();
};

