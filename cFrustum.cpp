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
	// 1: ���� ����� ���� ���� 8�� ����
	// 2: ��Ÿ ���� �ʱ�ȭ

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
	// 1: ���Ʈ���� �������Ǹ�Ʈ���� ����
	// 2: D3DXVec3Unproject(worldvertex, projvectex, viewport(NULL), projMatrix, viewMatrix, NULL);
	// 3: �� ���� D3DXPlaneFromPoints(�� ���� 3��)

	D3DXMATRIXA16 matView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProjection;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	for (int i = 0; i < m_vecProjVertex.size(); ++i)
	{
		D3DXVec3Unproject(&m_vecWorldVertex[i], &m_vecProjVertex[i], NULL, &matProjection, &matView, NULL);
	}

	//�յ� �¿� ���Ʒ�
	D3DXPlaneFromPoints(&m_vecPlane[0], &m_vecWorldVertex[0], &m_vecWorldVertex[1], &m_vecWorldVertex[2]);
	D3DXPlaneFromPoints(&m_vecPlane[1], &m_vecWorldVertex[5], &m_vecWorldVertex[4], &m_vecWorldVertex[7]);
	D3DXPlaneFromPoints(&m_vecPlane[2], &m_vecWorldVertex[4], &m_vecWorldVertex[0], &m_vecWorldVertex[6]);
	D3DXPlaneFromPoints(&m_vecPlane[3], &m_vecWorldVertex[1], &m_vecWorldVertex[5], &m_vecWorldVertex[3]);
	D3DXPlaneFromPoints(&m_vecPlane[4], &m_vecWorldVertex[4], &m_vecWorldVertex[5], &m_vecWorldVertex[0]);
	D3DXPlaneFromPoints(&m_vecPlane[5], &m_vecWorldVertex[7], &m_vecWorldVertex[6], &m_vecWorldVertex[3]);
}

bool cFrustum::IsIn(D3DXVECTOR3* vPosition, float fRadius)
{
	// �� �鿡 loop
	// �ȿ� �ִ��� �ۿ� �ִ��� �����ִ���...
	// D3DXPlaneDotCoord(������, ����) => �Ÿ� ��ȯ

	for (int i = 0; i < 6; ++i)
	{
		float d = D3DXPlaneDotCoord(&m_vecPlane[i], vPosition);
		
		if (d < fRadius);
		else return false;
	}

	return true;
}
