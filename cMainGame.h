#pragma once

class cCamera;

class cGrid;
class cPyramid;
class iMap;

class cCubePC;
class cCharacter;
class cCubeMan;

class cGroup;
class cFrame;
class cWoman;

class cMtlTex;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*		m_pCamera;

	//map
	cGrid*			m_pGrid;
	cPyramid*		m_pPyramid;
	vector<cGroup*> m_vecGroupMap;	//object map
	iMap*			m_pMap;

	//character
	cCubePC*		m_pCubePC;
	cCharacter*		m_pCubeMan;
	cFrame*			m_pRootFrame;
	cWoman*			m_pWoman;

	//mesh
	LPD3DXMESH		m_pMeshTeapot;
	LPD3DXMESH		m_pMeshSphere;
	D3DMATERIAL9	m_stMtlTeapot;
	D3DMATERIAL9	m_stMtlSphere;
	LPD3DXMESH				m_pMeshObjectMap;
	std::vector<cMtlTex*>	m_vecMtlTexObjectMap;

	//ray
	std::vector<ST_PNT_VERTEX>	m_vecVertexPlane;
	D3DMATERIAL9				m_stMtlPlane;
	LPDIRECT3DTEXTURE9			m_pTexturePlane;
	D3DXVECTOR3					m_vPickedPosition;

	//font
	LPD3DXFONT		m_pFont;
	ID3DXMesh*		m_p3DText;

public:
	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//light
	void Set_Light();

	//object
	void Obj_Render();
	void Load_Surface();

	//mesh
	void Setup_MeshObject();
	void Mesh_Render();

	//height map
	void Setup_HeightMap();

	//map for ray
	void Setup_Plane();
	void Plane_Render();

	//font
	void Create_Font();
	void Text_Render();
};

