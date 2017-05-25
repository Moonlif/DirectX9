#pragma once
#define ASTAR_X 16
#define ASTAR_Z 16

class cCamera;

class cGrid;
class cPyramid;
class iMap;
class cHeightMap;

class cCubePC;
class cCharacter;
class cCubeMan;

class cGroup;
class cFrame;
class cWoman;

class cMtlTex;

class cUI;

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
	iMap*			m_pMap;			//iMap*형으로 선언하면 릴리즈되지 않음. cHeightMap*형으로 하면 릴리즈 가능
									//다른방법으로 릴리즈 시킬 수 있는듯 한데 방법을 모르겠음

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

	//ui
	LPD3DXSPRITE			m_pSprite;
	D3DXIMAGE_INFO			m_stImageInfo;
	LPDIRECT3DTEXTURE9		m_pTexture;
	cUI*		m_pUI;

	//AStar
	std::vector<ST_PC_VERTEX>	m_vecVertexPlaneAstar;
	std::vector<D3DXVECTOR3> m_vecAstarRoute;
	ST_ASTAR m_aAstarTileOrigin[ASTAR_X][ASTAR_Z];
	ST_ASTAR m_aAstarTile[ASTAR_X][ASTAR_Z];
	POINT m_ptAstarStart;
	POINT m_ptAstarDest;
	std::vector<POINT> m_vecAstarOpenList;

	//xFile
	ID3DXMesh*						m_pMeshXFile;
	std::vector<D3DMATERIAL9>		m_vecMtlXFile;
	std::vector<IDirect3DTexture9*> m_vecTexXFile;

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

	//ui
	void Setup_UI();
	void UI_Render();

	//aStar
	void Setup_Astar();
	void Astar_Render();
	void Calculate_Astar();
	void Check_Astar(int targetX, int targetZ, int destX, int destZ);

	//xFile load
	void Setup_xFile();
	void xFile_Render();
};

