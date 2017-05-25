#include "stdafx.h"
#include "cMainGame.h"

#include "cCamera.h"

//map
#include "cGrid.h"
#include "cPyramid.h"
#include "cObjMap.h"
#include "cHeightMap.h"

//character
#include "cCubePC.h"
#include "cCubeMan.h"
#include "cGroup.h"
#include "cFrame.h"
#include "cWoman.h"

#include "cAseLoader.h"
#include "cObjLoader.h"

#include "cRay.h"

#include "cUI.h"

cMainGame::cMainGame()
	: m_pCamera(NULL)

	, m_pPyramid(NULL)
	, m_pGrid(NULL)
	, m_pMap(NULL)

	, m_pCubePC(NULL)
	, m_pCubeMan(NULL)
	, m_pRootFrame(NULL)
	, m_pWoman(NULL)

	, m_pMeshTeapot(NULL)
	, m_pMeshSphere(NULL)
	, m_pMeshObjectMap(NULL)

	, m_pFont(NULL)
	, m_p3DText(NULL)

	, m_pSprite(NULL)
	, m_pTexture(NULL)
	, m_pUI(NULL)

	, m_pMeshXFile(NULL)
{	 
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);

	//map
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pPyramid);
	SAFE_DELETE(m_pMap);
	SAFE_RELEASE(m_pTexturePlane);

	//character
	SAFE_DELETE(m_pCubePC);
	SAFE_DELETE(m_pCubeMan);
	for (int i = 0; i < m_vecGroupMap.size(); ++i) SAFE_DELETE(m_vecGroupMap[i]);
	if (m_pRootFrame) m_pRootFrame->Destroy();
	SAFE_DELETE(m_pWoman);

	//mesh
	SAFE_RELEASE(m_pMeshTeapot);
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pMeshObjectMap);
	for(int i=0; i<m_vecMtlTexObjectMap.size(); ++i) SAFE_RELEASE(m_vecMtlTexObjectMap[i]);

	//font
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_p3DText);

	//ui
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE(m_pUI);

	//xFile
	SAFE_RELEASE(m_pMeshXFile);

	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	//light
	Set_Light();

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);

	//map
	m_pGrid = new cGrid;
	m_pGrid->Setup(0, 0, ASTAR_X, ASTAR_Z, 1);
	//m_pPyramid = new cPyramid;
	//m_pPyramid->Setup();
	//cObjLoader loadObj;
	//loadObj.Load(m_vecGroupMap, "objects", "Map.obj");	//object map for render
	//Load_Surface();	//object m_pMap for collision
	//Setup_HeightMap(); //heightMap for collision & render
	//Setup_Plane();	//map for ray

	//character
	//m_pCubePC = new cCubePC;
	//m_pCubePC->Setup();
	//m_pCubeMan = new cCubeMan;
	//m_pCubeMan->Setup();
	//cAseLoader loadAse;
	//m_pRootFrame = loadAse.Load("woman/woman_01_all_stand.ASE");
	m_pWoman = new cWoman;
	m_pWoman->Setup();

	//mesh
	//Setup_MeshObject();	//create Mesh : teapot & sphere
	//cObjLoader objLoader;
	//m_pMeshObjectMap = objLoader.LoadMesh(m_vecMtlTexObjectMap, "objects", "Map.obj");	//object map by mesh

	//font
	//Create_Font();

	//ui
	//Setup_UI();
	//m_pUI = new cUI;
	//m_pUI->Setup();

	//astar
	Setup_Astar();

	//xFile
	Setup_xFile();

	m_pCamera->ReTarget(&m_pWoman->GetPosition());
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();

	if (m_pCubeMan) m_pCubeMan->Update(m_pMap);
	if (m_pRootFrame) m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);
	if (m_pWoman) m_pWoman->Update(m_pMap, m_vecVertexPlane);

	if (m_pUI) m_pUI->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);
	g_pD3DDevice->BeginScene();

	//map
	if (m_pGrid) m_pGrid->Render();
	//if (m_pPyramid) m_pPyramid->Render();
	//if (m_vecGroupMap.size() > 0) Obj_Render();
	if (m_pMap) m_pMap->Render();
	if (m_vecVertexPlane.size() > 0) Plane_Render();

	//character
	//if (m_pCubePC) m_pCubePC->Render();
	//if (m_pCubeMan) m_pCubeMan->Render();
	//if (m_pRootFrame) m_pRootFrame->Render();
	if (m_pWoman) m_pWoman->Render();

	//mesh
	//Mesh_Render();

	//text
	//Text_Render();

	//ui
	//UI_Render();
	if (m_pUI) m_pUI->Render();

	//astar
	Astar_Render();

	//xFile
	xFile_Render();

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera) m_pCamera->WndProc(hWnd, message, wParam, lParam);

	//ray picking
	switch (message)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
	{
		cRay r = cRay::RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
		for (int i = 0; i < m_vecVertexPlaneAstar.size(); i += 3)
		{
			D3DXVECTOR3 v(0, 0, 0);
			if (r.IntersectTri(m_vecVertexPlaneAstar[i + 0].p, m_vecVertexPlaneAstar[i + 1].p, m_vecVertexPlaneAstar[i + 2].p, v))
			{
				m_vPickedPosition = v;
			}
		}
		Calculate_Astar();
		if (m_pWoman) m_pWoman->SetRoute(m_vecAstarRoute);
	}
	break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	}
}


//light
void cMainGame::Set_Light()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	//g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));

	//directional light
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

	//Light test for each type
	{
		/*
		D3DLIGHT9 stLight;
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));

		//directional light (green)
		stLight.Type = D3DLIGHT_DIRECTIONAL;
		stLight.Ambient = D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f);
		stLight.Diffuse = D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f);
		stLight.Specular = D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f);

		D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
		D3DXVec3Normalize(&vDir, &vDir);
		stLight.Direction = vDir;

		g_pD3DDevice->SetLight(0, &stLight);
		g_pD3DDevice->LightEnable(0, true);


		//point light (blue)
		D3DLIGHT9 pointLight;
		ZeroMemory(&pointLight, sizeof(D3DLIGHT9));
		pointLight.Type = D3DLIGHT_POINT;
		pointLight.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pointLight.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pointLight.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pointLight.Position = D3DXVECTOR3(0.0f, 0.0f, 3.0f);
		pointLight.Range = 2.0f;

		pointLight.Attenuation0 = 0.0f;
		pointLight.Attenuation1 = 0.0f;
		pointLight.Attenuation2 = 0.0f;

		g_pD3DDevice->SetLight(1, &pointLight);
		g_pD3DDevice->LightEnable(1, true);

		//spot light (red)
		D3DLIGHT9 spotLight;
		ZeroMemory(&spotLight, sizeof(D3DLIGHT9));
		spotLight.Type = D3DLIGHT_SPOT;
		spotLight.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		spotLight.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		spotLight.Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		spotLight.Position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);
		spotLight.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		spotLight.Falloff = 1.0f;
		spotLight.Theta = D3DX_PI / 6.0f;
		spotLight.Phi = D3DX_PI / 4.0f;
		spotLight.Range = 5.0f;

		spotLight.Attenuation0 = 0.0f;
		spotLight.Attenuation1 = 0.0f;
		spotLight.Attenuation2 = 0.0f;

		g_pD3DDevice->SetLight(2, &spotLight);
		g_pD3DDevice->LightEnable(2, true);
		*/
	}
}


//object
void cMainGame::Obj_Render()
{
	D3DXMATRIXA16 matWorld, matS, matR;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
	matWorld = matS * matR;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	for each (auto p in m_vecGroupMap)
	{
		p->Render();
	}
}

void cMainGame::Load_Surface()
{
	D3DXMATRIXA16 matWorld, matS, matR;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
	matWorld = matS * matR;

	m_pMap = new cObjMap("objects", "map_surface.obj", &matWorld);
}


//mesh
void cMainGame::Setup_MeshObject()
{
	D3DXCreateTeapot(g_pD3DDevice, &m_pMeshTeapot, NULL);
	D3DXCreateSphere(g_pD3DDevice, 0.5f, 10, 10, &m_pMeshSphere, NULL);

	ZeroMemory(&m_stMtlTeapot, sizeof(D3DMATERIAL9));
	m_stMtlTeapot.Ambient = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	m_stMtlTeapot.Diffuse = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	m_stMtlTeapot.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);

	ZeroMemory(&m_stMtlSphere, sizeof(D3DMATERIAL9));
	m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
}

void cMainGame::Mesh_Render()
{
	D3DXMATRIXA16 matWorld, matS, matR, matT;

	//teapot render
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		matWorld = matS * matR;
		D3DXMatrixTranslation(&matWorld, 0, 0, 4);

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtlTeapot);
		m_pMeshTeapot->DrawSubset(0);
	}

	//sphere render
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtlSphere);
		m_pMeshSphere->DrawSubset(0);

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//map render
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
		D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
		matWorld = matS * matR * matT;
		
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		for (int i = 0; i < m_vecMtlTexObjectMap.size(); ++i)
		{
			g_pD3DDevice->SetTexture(0, m_vecMtlTexObjectMap[i]->GetTexture());
			g_pD3DDevice->SetMaterial(&m_vecMtlTexObjectMap[i]->GetMaterial());
			if(m_pMeshObjectMap) m_pMeshObjectMap->DrawSubset(i);
		}
	}
}

//heightMap
void cMainGame::Setup_HeightMap()
{
	cHeightMap *pMap = new cHeightMap;
	pMap->Setup("HeightMapData/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pMap;
}


//map for ray
void cMainGame::Setup_Plane()
{
	//plane setting
	{
		ST_PNT_VERTEX v;
		v.n = D3DXVECTOR3(0, -1, 0);

		v.p = D3DXVECTOR3(-10, 0, 10);
		v.t = D3DXVECTOR2(0, 0);
		m_vecVertexPlane.push_back(v);
		v.p = D3DXVECTOR3(10, 0, 10);
		v.t = D3DXVECTOR2(1, 0);
		m_vecVertexPlane.push_back(v);
		v.p = D3DXVECTOR3(10, 0, -10);
		v.t = D3DXVECTOR2(1, 1);
		m_vecVertexPlane.push_back(v);

		v.p = D3DXVECTOR3(-10, 0, 10);
		v.t = D3DXVECTOR2(0, 0);
		m_vecVertexPlane.push_back(v);
		v.p = D3DXVECTOR3(10, 0, -10);
		v.t = D3DXVECTOR2(1, 1);
		m_vecVertexPlane.push_back(v);
		v.p = D3DXVECTOR3(-10, 0, -10);
		v.t = D3DXVECTOR2(0, 1);
		m_vecVertexPlane.push_back(v);
	}

	D3DXCreateTextureFromFile(g_pD3DDevice, "steam.png", &m_pTexturePlane);

	ZeroMemory(&m_stMtlPlane, sizeof(D3DMATERIAL9));
	m_stMtlPlane.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stMtlPlane.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stMtlPlane.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void cMainGame::Plane_Render()
{
	D3DXMATRIXA16 matWorldPlane;
	D3DXMatrixIdentity(&matWorldPlane);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldPlane);
	g_pD3DDevice->SetTexture(0, m_pTexturePlane);
	g_pD3DDevice->SetMaterial(&m_stMtlPlane);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertexPlane.size() / 3, &m_vecVertexPlane[0], sizeof(ST_PNT_VERTEX));

}


//font
void cMainGame::Create_Font()
{
	//case 1: 2D
	{
		D3DXFONT_DESC fontDiscription;
		ZeroMemory(&fontDiscription, sizeof(D3DXFONT_DESC));

		fontDiscription.Height = 30;
		fontDiscription.Width = 15;
		fontDiscription.Weight = FW_MEDIUM;
		fontDiscription.Italic = false;
		fontDiscription.CharSet = DEFAULT_CHARSET;
		fontDiscription.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDiscription.PitchAndFamily = FF_DONTCARE;
		AddFontResource("font/umberto.ttf");
		strcpy_s(fontDiscription.FaceName, "umberto");

		D3DXCreateFontIndirect(g_pD3DDevice, &fontDiscription, &m_pFont);
	}

	//case 2: 3D
	{
		HDC hdc = CreateCompatibleDC(0);
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));
		lf.lfHeight = 25;
		lf.lfWidth = 12;
		lf.lfWeight = 500;	//0~1000 
		lf.lfItalic = false;
		lf.lfUnderline = false;
		lf.lfStrikeOut = false;
		lf.lfCharSet = DEFAULT_CHARSET;
		strcpy_s(lf.lfFaceName, "±¼¸²Ã¼");

		HFONT hFont;
		HFONT hFontOld;
		hFont = CreateFontIndirect(&lf);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
		D3DXCreateText(g_pD3DDevice, hdc, "Direct3D", 0.001f, 0.01f, &m_p3DText, 0, 0);
		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);
		DeleteDC(hdc);
	}
}

void cMainGame::Text_Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	//case 1: 2D
	{
		std::string sText;

		//mouse point
		sText = to_string(g_ptMouse.x) + ", " + to_string(g_ptMouse.y);

		//text position
		RECT rc;
		SetRect(&rc, 0, 100, 100, 100);

		if (m_pFont) m_pFont->DrawTextA(NULL, sText.c_str(), sText.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(88, 12, 8));

		//fps render
		std::string sText2 = to_string(g_nFps);
		SetRect(&rc, 0, 0, 0, 0);
		if (m_pFont) m_pFont->DrawTextA(NULL, sText2.c_str(), sText2.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(88, 12, 8));
	}

	//case 2: 3D
	{
		D3DXMATRIXA16 matWorld, matS, matR, matT;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 100.0f);
		D3DXMatrixRotationY(&matR, -D3DX_PI / 8.0f);
		D3DXMatrixTranslation(&matT, -2.0f, 2.0f, 0.0f);

		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_p3DText->DrawSubset(0);
	}
}


//ui
void cMainGame::Setup_UI()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	//±âÁ¸ texture¸¦ ¸¸µå´Â ¹æ½Ä
	{
		//m_pTexture = g_pTextureManager->GetTexture("UI/±èÅÂÈñ.jpg");
	}

	{
		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			"UI/±èÅÂÈñ.jpg",
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&m_stImageInfo,
			NULL,
			&m_pTexture);
	}
}

void cMainGame::UI_Render()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMATRIXA16 matS, matR, matT, mat;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixIdentity(&mat);
	
	{
		static float fAngle = 0.0f;
		fAngle += 0.01f;
		D3DXMatrixRotationZ(&matR, fAngle);
		D3DXMatrixTranslation(&matT, 200, 200, 0);
	}
	mat = matS * matR * matT;

	m_pSprite->SetTransform(&mat);

	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	
	//m_pSprite->Draw(m_pTexture, &rc, &D3DXVECTOR3(m_stImageInfo.Width /2, m_stImageInfo.Height/2, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(120, 255, 255, 255));

	m_pSprite->End();
}

void cMainGame::Setup_Astar()
{
	ZeroMemory(&m_aAstarTile, sizeof(m_aAstarTile));

	FILE *fp;
	fopen_s(&fp, "astar.txt", "r");

	int loadX = 0;
	int loadZ = 0;

	char szTemp[1024];
	fgets(szTemp, 1024, fp);
	while (true)
	{
		if (feof(fp)) break;

		//EBSD
		if (szTemp[loadX] == 'E')
		{
			m_aAstarTile[loadX][loadZ].state = ST_ASTAR::E_NONE;
		}
		else if (szTemp[loadX] == 'B')
		{
			m_aAstarTile[loadX][loadZ].state = ST_ASTAR::E_BLOCK;
		}
		else if (szTemp[loadX] == 'S')
		{
			m_aAstarTile[loadX][loadZ].state = ST_ASTAR::E_START;
		}
		else if (szTemp[loadX] == 'D')
		{
			m_aAstarTile[loadX][loadZ].state = ST_ASTAR::E_DESTINATION;
		}
		else
		{
			fgets(szTemp, 1024, fp);
			loadZ++;
			loadX = -1;
		}
		loadX++;
	}
	
	for (int i = 0; i < ASTAR_X; ++i)
	{
		for (int j = 0; j < ASTAR_Z; ++j)
		{
			m_aAstarTileOrigin[i][j] = m_aAstarTile[i][j];
		}
	}

	//vertext setting
	m_vecVertexPlaneAstar.clear();
	for (int z = 0; z < ASTAR_Z; ++z)
	{
		for (int x = 0; x < ASTAR_X; ++x)
		{
			ST_PC_VERTEX v;
			
			if (m_aAstarTile[x][z].state == ST_ASTAR::E_NONE)
			{
				v.c = D3DCOLOR_XRGB(255, 255, 255);
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_BLOCK)
			{
				v.c = D3DCOLOR_XRGB(100, 100, 100);
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_START)
			{
				v.c = D3DCOLOR_XRGB(255, 0, 0);
				if (m_pWoman) m_pWoman->SetPosition(D3DXVECTOR3(x + 0.5f, 0, z + 0.5f));
				m_ptAstarStart.x = x;
				m_ptAstarStart.y = z;
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_DESTINATION)
			{
				v.c = D3DCOLOR_XRGB(0, 0, 255);
				m_ptAstarDest.x = x;
				m_ptAstarDest.y = z;
			}

			v.p = D3DXVECTOR3(x, 0, z);
			m_vecVertexPlaneAstar.push_back(v);
			v.p = D3DXVECTOR3(x, 0, z + 1);
			m_vecVertexPlaneAstar.push_back(v);
			v.p = D3DXVECTOR3(x + 1, 0, z + 1);
			m_vecVertexPlaneAstar.push_back(v);
			v.p = D3DXVECTOR3(x, 0, z);
			m_vecVertexPlaneAstar.push_back(v);
			v.p = D3DXVECTOR3(x + 1, 0, z + 1);
			m_vecVertexPlaneAstar.push_back(v);
			v.p = D3DXVECTOR3(x + 1, 0, z);
			m_vecVertexPlaneAstar.push_back(v);
		}
	}
}

void cMainGame::Astar_Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIXA16 matWorldAstar;
	D3DXMatrixIdentity(&matWorldAstar);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldAstar);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertexPlaneAstar.size() / 3, &m_vecVertexPlaneAstar[0], sizeof(ST_PC_VERTEX));
}

void cMainGame::Calculate_Astar()
{
	for (int i = 0; i < ASTAR_X; ++i)
	{
		for (int j = 0; j < ASTAR_Z; ++j)
		{
			m_aAstarTile[i][j] = m_aAstarTileOrigin[i][j];
		}
	}

	//set vecRoute
	//m_ptAstarStart.x = m_pWoman->GetPosition().x;
	//m_ptAstarStart.y = m_pWoman->GetPosition().z;
	//m_ptAstarDest.x = m_vPickedPosition.x;
	//m_ptAstarDest.y = m_vPickedPosition.z;

	m_vecAstarRoute.clear();
	m_vecAstarOpenList.clear();
	{
		//if dest is wrong, return
		if (m_aAstarTile[m_ptAstarDest.x][m_ptAstarDest.y].state == ST_ASTAR::E_BLOCK) return;

		//check the tile
		while (true)
		{
			if (m_aAstarTile[m_ptAstarDest.x][m_ptAstarDest.y].state == ST_ASTAR::E_OPEN) break;

			POINT ptCurrent;
			if (m_vecAstarOpenList.size() == 0)
			{
				if (m_aAstarTile[m_ptAstarStart.x][m_ptAstarStart.y].state == ST_ASTAR::E_CLOSE) break;
				else ptCurrent = m_ptAstarStart;
			}
			else //there is openlist
			{
				std::vector<POINT>::iterator eraseit;
				std::vector<POINT>::iterator it;
				for (it = m_vecAstarOpenList.begin(); it != m_vecAstarOpenList.end(); ++it)
				{
					//first of list
					if (it == m_vecAstarOpenList.begin())
					{
						ptCurrent = *it;
						eraseit = it;
					}
					//compare f value
					else if (m_aAstarTile[it->x][it->y].f < m_aAstarTile[ptCurrent.x][ptCurrent.y].f)
					{
						ptCurrent = *it;
						eraseit = it;
					}
					//if f value is same, compare h value
					else if (m_aAstarTile[it->x][it->y].f == m_aAstarTile[ptCurrent.x][ptCurrent.y].f)
					{
						if (m_aAstarTile[it->x][it->y].h < m_aAstarTile[ptCurrent.x][ptCurrent.y].h)
						{
							ptCurrent = *it;
							eraseit = it;
						}
					}
				}
				m_vecAstarOpenList.erase(eraseit);
			}

			Check_Astar(ptCurrent.x, ptCurrent.y, m_ptAstarDest.x, m_ptAstarDest.y);
		}

		//if find the route, push_back the node
		if (m_aAstarTile[m_ptAstarDest.x][m_ptAstarDest.y].state == ST_ASTAR::E_OPEN)
		{
			while (true)
			{
				if (m_vecAstarRoute.size() == 0)
				{
					m_vecAstarRoute.push_back(D3DXVECTOR3(m_ptAstarDest.x + 0.5f, 0, m_ptAstarDest.y + 0.5f));
				}
				else
				{
					//add node that last node's parent
					m_aAstarTile[(int)m_vecAstarRoute.back().x][(int)m_vecAstarRoute.back().z].state = ST_ASTAR::E_ROUTE;
					POINT routeParent = m_aAstarTile[(int)m_vecAstarRoute.back().x][(int)m_vecAstarRoute.back().z].parent;
					m_vecAstarRoute.push_back(D3DXVECTOR3(routeParent.x + 0.5f, 0, routeParent.y + 0.5f));

					if (routeParent.x == m_ptAstarStart.x && routeParent.y == m_ptAstarStart.y) break;
				}
			}
		}
	}

	//set color
	m_aAstarTile[m_ptAstarStart.x][m_ptAstarStart.y].state = ST_ASTAR::E_START;
	m_aAstarTile[m_ptAstarDest.x][m_ptAstarDest.y].state = ST_ASTAR::E_DESTINATION;
	for (int z = 0; z < ASTAR_Z; ++z)
	{
		for (int x = 0; x < ASTAR_X; ++x)
		{
			if (m_aAstarTile[x][z].state == ST_ASTAR::E_NONE)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(255, 255, 255);
				}
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_BLOCK)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(50, 50, 50);
				}
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_START)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(255, 255, 0);
				}
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_DESTINATION)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(0, 0, 255);
				}
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_OPEN)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(0, 255, 0);
				}
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_CLOSE)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(255, 0, 0);
				}
			}
			else if (m_aAstarTile[x][z].state == ST_ASTAR::E_ROUTE)
			{
				for (int i = 0; i < 6; ++i)
				{
					m_vecVertexPlaneAstar[6 * (x + z*ASTAR_X) + i].c = D3DCOLOR_XRGB(0, 255, 255);
				}
			}
		}
	}
}

void cMainGame::Check_Astar(int targetX, int targetZ, int destX, int destZ)
{
	m_aAstarTile[targetX][targetZ].state = ST_ASTAR::E_CLOSE;

	int nearX, nearZ;
	
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			nearX = targetX + j;
			nearZ = targetZ + i;

			if (nearX >= 0 && nearX < ASTAR_X && nearZ >= 0 && nearZ < ASTAR_Z)
			{
				//cant through block
				{
					if (i == -1 && j == -1)
					{
						if (m_aAstarTile[nearX + 1][nearZ].state == ST_ASTAR::E_BLOCK) continue;
						if (m_aAstarTile[nearX][nearZ + 1].state == ST_ASTAR::E_BLOCK) continue;
					}
					else if (i == -1 && j == 1)
					{
						if (m_aAstarTile[nearX - 1][nearZ].state == ST_ASTAR::E_BLOCK) continue;
						if (m_aAstarTile[nearX][nearZ + 1].state == ST_ASTAR::E_BLOCK) continue;
					}
					else if (i == 1 && j == -1)
					{
						if (m_aAstarTile[nearX + 1][nearZ].state == ST_ASTAR::E_BLOCK) continue;
						if (m_aAstarTile[nearX][nearZ - 1].state == ST_ASTAR::E_BLOCK) continue;
					}
					else if (i == 1 && j == 1)
					{
						if (m_aAstarTile[nearX - 1][nearZ].state == ST_ASTAR::E_BLOCK) continue;
						if (m_aAstarTile[nearX][nearZ - 1].state == ST_ASTAR::E_BLOCK) continue;
					}
				}

				float cost;
				if ((i + j) % 2 == 0) cost = 1.4f;
				else cost = 1.0f;

				ST_ASTAR tempAstar;
				POINT tempPt, parentPt;
				tempAstar.g = m_aAstarTile[targetX][targetZ].g + cost;
				tempAstar.h = sqrt((destX - nearX)*(destX - nearX) + (destZ - nearZ)*(destZ - nearZ));
				tempAstar.f = tempAstar.g + tempAstar.h;
				tempAstar.state = ST_ASTAR::E_OPEN;
				parentPt.x = targetX; parentPt.y = targetZ;
				tempAstar.parent = parentPt;

				if (m_aAstarTile[nearX][nearZ].state == ST_ASTAR::E_NONE)
				{
					m_aAstarTile[nearX][nearZ] = tempAstar;

					tempPt.x = nearX; tempPt.y = nearZ;
					m_vecAstarOpenList.push_back(tempPt);
				}
				else if (m_aAstarTile[nearX][nearZ].state == ST_ASTAR::E_OPEN)
				{
					if (tempAstar.f < m_aAstarTile[nearX][nearZ].f)
					{
						m_aAstarTile[nearX][nearZ] = tempAstar;

						tempPt.x = nearX; tempPt.y = nearZ;
						m_vecAstarOpenList.push_back(tempPt);
					}
				}
				else if (m_aAstarTile[nearX][nearZ].state == ST_ASTAR::E_DESTINATION)
				{
					m_aAstarTile[nearX][nearZ] = tempAstar;
					break;
				}
			}
		}
	}
}

void cMainGame::Setup_xFile()
{
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	D3DXLoadMeshFromX("Objects/Zealot/zealot.x", D3DXMESH_MANAGED, g_pD3DDevice, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &m_pMeshXFile);

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; ++i)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			m_vecMtlXFile.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;

				string sFileName = mtrls[i].pTextureFilename;
				string sFullPath = "objects/zealot/" + sFileName;
				D3DXCreateTextureFromFile(g_pD3DDevice, sFullPath.c_str(), &tex);
				m_vecTexXFile.push_back(tex);
			}
			else
			{
				m_vecTexXFile.push_back(0);
			}
		}
	}

	mtrlBuffer->Release();
	adjBuffer->Release();
}

void cMainGame::xFile_Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	static float y = 0.0f;
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, y);
	y += 0.01f;

	if (y >= D3DX_PI * 2) y = 0.0f;
	
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matR);

	for (int i = 0; i < m_vecMtlXFile.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtlXFile[i]);
		g_pD3DDevice->SetTexture(0, m_vecTexXFile[i]);
		m_pMeshXFile->DrawSubset(i);
	}
}
