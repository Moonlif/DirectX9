#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16 * pmatParent)
{
	D3DXMATRIXA16 matR, matT;
	{
		D3DXMatrixIdentity(&matR);
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;


		D3DXMatrixIdentity(&matT);
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
	}
	//회전
	//D3DXQuaternionSlerp();
	//D3DXMatrixRotationQuaternion();
	D3DXQUATERNION qRot;
	//D3DXQuaternionSlerp(&qRot, &m_vecRotTrack[nKeyFrame], &m_vecRotTrack[nKeyFrame + 1], )
	//위치 : D3DXVec3Lerp;

	//m_matLocalTM = matR * matT;
	m_matWorldTM = m_matLocalTM;

	if (pmatParent)
	{
		m_matWorldTM *= (*pmatParent);
	}

	for each (auto c in m_vecChild)
	{
		c->Update(nKeyFrame, &m_matWorldTM);
	}
}

void cFrame::Render()
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));
	}

	for each (auto c in m_vecChild)
		c->Render();
}

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each (auto c in m_vecChild)
		c->Destroy();

	this->Release();
}

void cFrame::CalcOriginalLocalTM(D3DXMATRIXA16 * pmatParent)
{
	m_matLocalTM = m_matWorldTM;

	if (pmatParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each (auto c in m_vecChild)
		c->CalcOriginalLocalTM(&m_matWorldTM);
}