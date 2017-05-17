#include "stdafx.h"
#include "cMainGame.h"

#include "cCubePC.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cPyramid.h"

#include "cCubeMan.h"

#include "cGroup.h"
#include "cObjLoader.h"
#include "cObjMap.h"

#include "cFrame.h"
#include "cAseLoader.h"
#include "cWoman.h"

cMainGame::cMainGame()
	: m_pCubePC(NULL)
	, m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pTexture(NULL)
	, m_pCubeMan(NULL)
	, m_pCubeMan2(NULL)
	, m_pMap(NULL)
	, m_pRootFrame(NULL)
	, m_pWoman(NULL)
	, m_pFont(NULL)
	, m_p3DText(NULL)
	, m_pMeshTeapot(NULL)
	, m_pMeshSphere(NULL)
	, m_pMeshObjectMap(NULL)
{	
	m_bSelectWoman = true;
	m_vDestinationSphere = D3DXVECTOR3(0, 0.5f, 0);
	m_vPositionSphere = D3DXVECTOR3(0, 0.5f, 0);
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCubePC);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pPyramid);
	SAFE_DELETE(m_pCubeMan);
	SAFE_DELETE(m_pCubeMan2);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pWoman);

	if (m_pRootFrame) m_pRootFrame->Destroy();
	for (int i = 0; i < m_vecGroup.size(); ++i)
	{
		SAFE_DELETE(m_vecGroup[i]);
	}

	//font
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_p3DText);

	//texture test
	SAFE_RELEASE(m_pTexture);

	//mesh
	SAFE_RELEASE(m_pMeshTeapot);
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pMeshObjectMap);
	for(int i=0; i<m_vecMtlTexObjectMap.size(); ++i)
	{
		SAFE_RELEASE(m_vecMtlTexObjectMap[i]);
	}

	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	//texture test setting
	D3DXCreateTextureFromFile(g_pD3DDevice, "steam.png", &m_pTexture);

	//m_pCubeMan = new cCubeMan;
	//m_pCubeMan->Setup(false);

	//cObjLoader loadObj;
	//loadObj.Load(m_vecGroup, "objects", "Map.obj");
	//Load_Surface();

	//cAseLoader loadAse;
	//m_pRootFrame = loadAse.Load("woman/woman_01_all_stand.ASE");

	m_pWoman = new cWoman;
	m_pWoman->Setup();
	m_pWoman->SetMtlSphere(m_bSelectWoman);

	m_pCamera = new cCamera;
	m_pCamera->Setup(&m_pWoman->GetPosition());

	m_pGrid = new cGrid;
	m_pGrid->Setup(20, 20, 1);
	m_pPyramid = new cPyramid;
	m_pPyramid->Setup();

	//light
	Set_Light();

	//font
	Create_Font();

	//mesh test
	Setup_MeshObject();

	cObjLoader objLoader;
	m_pMeshObjectMap = objLoader.LoadMesh(m_vecMtlTexObjectMap, "objects", "Map.obj");

	//floor setting
	{
		ST_PNT_VERTEX v;
		v.n = D3DXVECTOR3(0, -1, 0);

		v.p = D3DXVECTOR3(-10, 0, 10);
		v.t = D3DXVECTOR2(0, 0);
		m_vecFloorVertex.push_back(v);
		v.p = D3DXVECTOR3(10, 0, 10);
		v.t = D3DXVECTOR2(1, 0);
		m_vecFloorVertex.push_back(v);
		v.p = D3DXVECTOR3(10, 0, -10);
		v.t = D3DXVECTOR2(1, 1);
		m_vecFloorVertex.push_back(v);

		v.p = D3DXVECTOR3(-10, 0, 10);
		v.t = D3DXVECTOR2(0, 0);
		m_vecFloorVertex.push_back(v);
		v.p = D3DXVECTOR3(10, 0, -10);
		v.t = D3DXVECTOR2(1, 1);
		m_vecFloorVertex.push_back(v);
		v.p = D3DXVECTOR3(-10, 0, -10);
		v.t = D3DXVECTOR2(0, 1);
		m_vecFloorVertex.push_back(v);
	}
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();

	//if (m_pCubeMan) m_pCubeMan->Update(m_pMap);
	//if (m_pRootFrame) m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);
	if (m_pWoman) m_pWoman->Update(NULL);

	//lclick targeting
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		D3DVIEWPORT9 vp;
		g_pD3DDevice->GetViewport(&vp);

		D3DXMATRIXA16 matProj;
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		float px, py;
		px = (((2.0f * g_ptMouse.x) / vp.Width) - 1.0f) / matProj(0, 0);
		py = (((-2.0f * g_ptMouse.y) / vp.Height) + 1.0f) / matProj(1, 1);

		D3DXVECTOR3 vRayPosition(0, 0, 0);
		D3DXVECTOR3 vRayDirection(px, py, 1.0f);

		D3DXMATRIXA16 matView, matViewInverse;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matViewInverse, 0, &matView);

		D3DXVec3TransformCoord(&vRayPosition, &vRayPosition, &matViewInverse);
		D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matViewInverse);
		D3DXVec3Normalize(&vRayDirection, &vRayDirection);

		//collision
		BOOL hitSphere = false;
		{
			D3DXVECTOR3 v = vRayPosition - m_vPositionSphere;
			float b = 2.0f* D3DXVec3Dot(&vRayDirection, &v);
			float c = D3DXVec3Dot(&v, &v) - (0.5f * 0.5f);

			float discriminant = (b * b) - (4.0f * c);

			if (discriminant > 0.0f)
			{
				discriminant = sqrt(discriminant);

				float s0 = (-b + discriminant) / 2.0f;
				float s1 = (-b - discriminant) / 2.0f;
				if (s0 >= 0.0f || s1 >= 0.0f) hitSphere = true;
			}
			if (hitSphere == true) m_bSelectWoman = false;
		}

		BOOL hitWoman = false;
		{
			D3DXVECTOR3 vWoman = m_pWoman->GetPosition();
			vWoman.y += 0.5f;
			D3DXVECTOR3 v = vRayPosition - vWoman;
			float b = 2.0f* D3DXVec3Dot(&vRayDirection, &v);
			float c = D3DXVec3Dot(&v, &v) - (0.5f * 0.5f); //r*r

			float discriminant = (b * b) - (4.0f * c);

			if (discriminant > 0.0f)
			{
				discriminant = sqrt(discriminant);

				float s0 = (-b + discriminant) / 2.0f;
				float s1 = (-b - discriminant) / 2.0f;
				if (s0 >= 0.0f || s1 >= 0.0f) hitWoman = true;
			}
			if (hitWoman == true) m_bSelectWoman = true;
		}

		//change color
		m_pWoman->SetMtlSphere(m_bSelectWoman);
		if (m_bSelectWoman)
		{
			m_pCamera->Setup(&m_pWoman->GetPosition());
			m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
			m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
			m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
		}
		else
		{
			m_pCamera->Setup(&m_vPositionSphere);
			m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
		}
	}

	//rclick moving
	if (GetKeyState(VK_RBUTTON) & 0x8000)
	{
		D3DVIEWPORT9 vp;
		g_pD3DDevice->GetViewport(&vp);

		D3DXMATRIXA16 matProj;
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		float px, py;
		px = (((2.0f * g_ptMouse.x) / vp.Width) - 1.0f) / matProj(0, 0);
		py = (((-2.0f * g_ptMouse.y) / vp.Height) + 1.0f) / matProj(1, 1);

		D3DXVECTOR3 vRayPosition(0, 0, 0);
		D3DXVECTOR3 vRayDirection(px, py, 1.0f);

		D3DXMATRIXA16 matView, matViewInverse;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matViewInverse, 0, &matView);

		D3DXVec3TransformCoord(&vRayPosition, &vRayPosition, &matViewInverse);
		D3DXVec3TransformNormal(&vRayDirection, &vRayDirection, &matViewInverse);
		D3DXVec3Normalize(&vRayDirection, &vRayDirection);

		for (size_t i = 0; i < m_vecFloorVertex.size(); i += 3)
		{
			float u, v, f;
			D3DXVECTOR3 p0 = m_vecFloorVertex[i + 0].p;
			D3DXVECTOR3 p1 = m_vecFloorVertex[i + 1].p;
			D3DXVECTOR3 p2 = m_vecFloorVertex[i + 2].p;

			if (D3DXIntersectTri(&p0, &p1, &p2, &vRayPosition, &vRayDirection, &u, &v, &f))
			{
				D3DXVECTOR3 vPoint = p0 + u*(p1 - p0) + v*(p2 - p0);
				if (m_bSelectWoman) m_pWoman->SetDestination(D3DXVECTOR3(vPoint.x, 0, vPoint.z));
				else m_vDestinationSphere = D3DXVECTOR3(vPoint.x, 0.5f, vPoint.z);
				break;
			}
		}
	}

	//move the sphere
	D3DXVECTOR3 dis = m_vDestinationSphere - m_vPositionSphere;
	if (D3DXVec3Length(&dis) >= 0.1f)
	{
		D3DXVECTOR3 dir;
		D3DXVec3Normalize(&dir, &dis);
		m_vPositionSphere += dir * 0.05f;
	}
	else m_vPositionSphere = m_vDestinationSphere;
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);
	g_pD3DDevice->BeginScene();

	//if (m_pGrid) m_pGrid->Render();
	if (m_pPyramid) m_pPyramid->Render();

	//floor render
	{
		D3DXMATRIXA16 matFloorWorld;
		D3DXMatrixIdentity(&matFloorWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matFloorWorld);
		g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecFloorVertex.size() / 3, &m_vecFloorVertex[0], sizeof(ST_PNT_VERTEX));
	}

	//if (m_pCubeMan) m_pCubeMan->Render();
	//if (m_pRootFrame) m_pRootFrame->Render();

	m_dwTime1 = GetTickCount();
	if (m_pWoman) m_pWoman->Render();
	m_dwTime2 = GetTickCount();
	m_dwTime3 = GetTickCount();

	Text_Render();

	//Obj_Render();
	Mesh_Render();

	//texture test render
	{
		//D3DXMATRIXA16 matWorld;
		//D3DXMatrixIdentity(&matWorld);
		//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//g_pD3DDevice->SetTexture(0, m_pTexture);
		//g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
		//g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PT_VERTEX));
		//g_pD3DDevice->SetTexture(0, NULL);
	}

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera) m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

//light
void cMainGame::Set_Light()
{
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));

	//directional light (green)
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

//object
void cMainGame::Obj_Render()
{
	D3DXMATRIXA16 matWorld, matS, matR;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
	matWorld = matS * matR;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	for each (auto p in m_vecGroup)
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
		strcpy_s(lf.lfFaceName, "굴림체");
		
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
		std::string sText; // ("ABC 123 !@# 가나다라");
		std::string sTick1 = to_string(m_dwTime2 - m_dwTime1);
		std::string sTick2 = to_string(m_dwTime3 - m_dwTime2);

		sText = sTick1 + " / " + sTick2;

		//mouse point
		sText = to_string(g_ptMouse.x) + ", " + to_string(g_ptMouse.y);

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
		//m_p3DText->DrawSubset(0);
	}
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
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DXMATRIXA16 matWorld, matS, matR, matT;

	//teapot render
	{
		//D3DXMatrixIdentity(&matWorld);
		//D3DXMatrixIdentity(&matS);
		//D3DXMatrixIdentity(&matR);

		//D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		//matWorld = matS * matR;
		//D3DXMatrixTranslation(&matWorld, 0, 0, 4);

		//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//g_pD3DDevice->SetMaterial(&m_stMtlTeapot);
		//m_pMeshTeapot->DrawSubset(0);
	}

	//sphere render
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		D3DXMatrixTranslation(&matT, m_vPositionSphere.x, m_vPositionSphere.y, m_vPositionSphere.z);
		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtlSphere);
		m_pMeshSphere->DrawSubset(0);

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//map render
	{
		//D3DXMatrixIdentity(&matWorld);
		//D3DXMatrixIdentity(&matS);
		//D3DXMatrixIdentity(&matR);
		//D3DXMatrixIdentity(&matT);

		//D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
		//D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
		////D3DXMatrixTranslation(&matT, 20, 0, 0);
		//matWorld = matS * matR * matT;
		//
		//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		//for (int i = 0; i < m_vecMtlTexObjectMap.size(); ++i)
		//{
		//	g_pD3DDevice->SetTexture(0, m_vecMtlTexObjectMap[i]->GetTexture());
		//	g_pD3DDevice->SetMaterial(&m_vecMtlTexObjectMap[i]->GetMaterial());
		//	if(m_pMeshObjectMap) m_pMeshObjectMap->DrawSubset(i);
		//}
	}
}