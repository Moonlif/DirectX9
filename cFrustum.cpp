#include "stdafx.h"
#include "cFrustum.h"


cFrustum::cFrustum()
{
}


cFrustum::~cFrustum()
{
}

void cFrustum::Setup()
{
	// 1: 상자 만들기 위한 정점 8개 설정
	// 2: 기타 변수 초기화

	m_vecProjVertex.clear();
	m_vecWorldVertex.clear();
	m_vecPlane.clear();

	m_vecProjVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	m_vecProjVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	m_vecProjVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, 0.0f));
	m_vecProjVertex.push_back(D3DXVECTOR3(1.0f, -1.0f, 0.0f));

	m_vecProjVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	m_vecProjVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecProjVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	m_vecProjVertex.push_back(D3DXVECTOR3(1.0f, -1.0f, 1.0f));

	m_vecWorldVertex.resize(8);
	m_vecPlane.resize(6);
}

void cFrustum::Update()
{
	// 1: 뷰매트릭스 프로젝션매트릭스 구함
	// 2: D3DXVec3Unproject(worldvertex, projvectex, viewport(NULL), projMatrix, viewMatrix, NULL);
	// 3: 면 생성 D3DXPlaneFromPoints(각 정점 3개)

	D3DXMATRIXA16 matView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProjection;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	for (int i = 0; i < m_vecProjVertex.size(); ++i)
	{
		D3DXVec3Unproject(&m_vecWorldVertex[i], &m_vecProjVertex[i], NULL, &matProjection, &matView, NULL);
	}

	//앞뒤 좌우 위아래
	D3DXPlaneFromPoints(&m_vecPlane[0], &m_vecWorldVertex[0], &m_vecWorldVertex[1], &m_vecWorldVertex[2]);
	D3DXPlaneFromPoints(&m_vecPlane[1], &m_vecWorldVertex[5], &m_vecWorldVertex[4], &m_vecWorldVertex[7]);
	D3DXPlaneFromPoints(&m_vecPlane[2], &m_vecWorldVertex[4], &m_vecWorldVertex[0], &m_vecWorldVertex[6]);
	D3DXPlaneFromPoints(&m_vecPlane[3], &m_vecWorldVertex[1], &m_vecWorldVertex[5], &m_vecWorldVertex[3]);
	D3DXPlaneFromPoints(&m_vecPlane[4], &m_vecWorldVertex[4], &m_vecWorldVertex[5], &m_vecWorldVertex[0]);
	D3DXPlaneFromPoints(&m_vecPlane[5], &m_vecWorldVertex[7], &m_vecWorldVertex[6], &m_vecWorldVertex[3]);
}

bool cFrustum::IsIn(D3DXVECTOR3* vPosition, float fRadius)
{
	// 각 면에 loop
	// 안에 있는지 밖에 있는지 걸쳐있는지...
	// D3DXPlaneDotCoord(면정보, 정점) => 거리 반환

	for (int i = 0; i < 6; ++i)
	{
		float d = D3DXPlaneDotCoord(&m_vecPlane[i], vPosition);
		
		if (d < fRadius);
		else return false;
	}

	return true;
}
