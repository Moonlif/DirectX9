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
	//m_pGrid = new cGrid;
	//m_pGrid->Setup(20, 20, 1);
	//m_pPyramid = new cPyramid;
	//m_pPyramid->Setup();
	//cObjLoader loadObj;
	//loadObj.Load(m_vecGroupMap, "objects", "Map.obj");	//object map for render
	//Load_Surface();	//object m_pMap for collision
	//Setup_HeightMap(); //heightMap for collision & render
	Setup_Plane();	//map for ray

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
	Setup_UI();
	m_pUI = new cUI;
	m_pUI->Setup();

	m_pCamera->ReTarget(&m_pWoman->GetPosition());
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();

	if (m_pCubeMan) m_pCubeMan->Update(m_pMap);
	if (m_pRootFrame) m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);
	if (m_pWoman) m_pWoman->Update(m_pMap, m_vecVertexPlane);

	if (m_pUI->m_isHidden == false) m_pUI->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);
	g_pD3DDevice->BeginScene();

	//map
	//if (m_pGrid) m_pGrid->Render();
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
	UI_Render();
	if (m_pUI->m_isHidden == false) m_pUI->Render();

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pUI->m_isMouseOnUI == false || m_pUI->m_isHidden == true)
	{
		if (m_pCamera) m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
	//ray picking
	switch (message)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		m_pUI->m_isHidden = false;
	{
		//cRay r = cRay::RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
		//for (int i = 0; i < m_vecVertexPlane.size(); i += 3)
		//{
		//	D3DXVECTOR3 v(0, 0, 0);
		//	if (r.IntersectTri(m_vecVertexPlane[i + 0].p, m_vecVertexPlane[i + 1].p, m_vecVertexPlane[i + 2].p, v))
		//	{
		//		m_vPickedPosition = v;
		//	}
		//}
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
	D3DXMATRIXA16 matFloorWorld;
	D3DXMatrixIdentity(&matFloorWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matFloorWorld);
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
