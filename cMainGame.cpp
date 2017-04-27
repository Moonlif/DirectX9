#include "stdafx.h"
#include "cMainGame.h"

#include "cCubePC.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cPyramid.h"

#include "cCubeMan.h"
#include "cObjLoader.h"

cMainGame::cMainGame()
	: m_pCubePC(NULL)
	, m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pTexture(NULL)
	, m_pCubeMan(NULL)
	, m_pObjLoader(NULL)
	//, m_pCubeMan2(NULL)
{	
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCubePC);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pTexture);

	SAFE_DELETE(m_pCubeMan);
	SAFE_DELETE(m_pObjLoader);
	//SAFE_DELETE(m_pCubeMan2);

	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	//texture setting
	{
		D3DXCreateTextureFromFile(g_pD3DDevice, "steam.png", &m_pTexture);
		ST_PT_VERTEX v;
		v.p = D3DXVECTOR3(0, 0, 0);
		v.t = D3DXVECTOR2(0, 1.0f);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2(0, 0.5f);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(1, 0, 0);
		v.t = D3DXVECTOR2(1, 1);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(0, 0, 0);
		v.t = D3DXVECTOR2(0, 1.0f);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2(0, 0.5f);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(1, 0, 0);
		v.t = D3DXVECTOR2(1, 1);
		m_vecVertex.push_back(v);

		D3DXMATRIXA16 matZTrans;
		D3DXMatrixTranslation(&matZTrans, 0.0f, 0.0f, 3.0f);
		for (int i = 0; i < 3; ++i)
		{
			D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &matZTrans);
		}
		D3DXMatrixTranslation(&matZTrans, -3.0f, 0.0f, 0.0f);
		for (int i = 3; i < 6; ++i)
		{
			D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &matZTrans);
		}
	}

	m_pCubePC = new cCubePC;
	m_pCubePC->Setup();

	m_pCubeMan = new cCubeMan;
	m_pCubeMan->Setup(false);

	m_pObjLoader = new cObjLoader;
	m_pObjLoader->LoadObj();

	//m_pCubeMan2 = new cCubeMan;
	//m_pCubeMan2->Setup(true);

	m_pCamera = new cCamera;
	//m_pCamera->Setup(&m_pCubePC->GetPosition());
	m_pCamera->Setup(&m_pCubeMan->GetPosition());
	//m_pCamera->Setup(&m_vCamTarget);

	m_pGrid = new cGrid;
	m_pGrid->Setup(20, 20, 1.0f);

	m_pPyramid = new cPyramid;
	m_pPyramid->Setup();

	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	Set_Light();
}

void cMainGame::Update()
{
	//if (m_pCubePC) m_pCubePC->Update();
	if (m_pCubeMan) m_pCubeMan->Update();
	//if (m_pCubeMan2) m_pCubeMan2->Update();
	if (m_pCamera) m_pCamera->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100,100,100), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	
	{
		if (m_pGrid) m_pGrid->Render();
		//if (m_pCubePC) m_pCubePC->Render();
		if (m_pPyramid) m_pPyramid->Render();

		if (m_pCubeMan) m_pCubeMan->Render();
		if (m_pObjLoader) m_pObjLoader->Render();
		//if (m_pCubeMan2) m_pCubeMan2->Render();
	}

	//texture test
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

void cMainGame::Set_Light()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));

	//directional light (green)
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

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
