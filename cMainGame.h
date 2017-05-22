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
class cMtlTex;

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

	//mesh
	LPD3DXMESH		m_pMeshTeapot;
	LPD3DXMESH		m_pMeshSphere;
	D3DMATERIAL9	m_stMtlTeapot;
	D3DMATERIAL9	m_stMtlSphere;
	LPD3DXMESH				m_pMeshObjectMap;
	std::vector<cMtlTex*>	m_vecMtlTexObjectMap;

	//ray test
	std::vector<ST_SPHERE>	m_vecSphere;
	D3DMATERIAL9			m_stMtlNone;
	D3DMATERIAL9			m_stMtlPicked;
	D3DMATERIAL9			m_stMtlPlane;
	D3DXVECTOR3				m_vPickedPosition;
	std::vector<ST_PNT_VERTEX>	m_vecPlaneVertex;

	//texture test
	LPDIRECT3DTEXTURE9			m_pTexture;
	std::vector<ST_PT_VERTEX>	m_vecVertex;

	//timecheck
	DWORD m_dwTime1;
	DWORD m_dwTime2;
	DWORD m_dwTime3;

	//height map
	LPD3DXMESH			m_pMeshHeightMap;
	LPDIRECT3DTEXTURE9	m_pTextureHeightMap;
	std::vector<ST_PNT_VERTEX> m_vecVertexHeightMap;

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

	//mesh
	void Setup_MeshObject();
	void Mesh_Render();

	//ray test
	void Setup_pickingObj();
	void PickingObj_Render();

	//height map
	void Setup_heightMap();
	void Render_heightMap();
};

