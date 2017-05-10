#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
	: m_pMtlTex(NULL)
	, m_dwFirstFrame(0)
	, m_dwLastFrame(0)
	, m_dwFrameSpeed(0)
	, m_dwTicksPerFrame(0)
	, m_dwPreTime(0)
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
		matR._44 = 1.0f;
		CalcLocalR(nKeyFrame, matR);

		D3DXMatrixIdentity(&matT);
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
		CalcLocalT(nKeyFrame, matT);
	}

	m_matWorldTM = matR * matT;

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


int cFrame::GetKeyFrame()
{
	DWORD time = timeGetTime();

	DWORD firstTime = m_dwFirstFrame * m_dwTicksPerFrame;
	DWORD lastTime = m_dwLastFrame * m_dwTicksPerFrame;
	DWORD first2last = lastTime - firstTime;

	//if (m_dwPreTime + m_dwFrameSpeed > time)
	//{
	//	return (m_dwPreTime % first2last) + firstTime;
	//}
	//else
	//{
	//	m_dwPreTime = time;
	//	return (time % first2last) + firstTime;
	//}

	return (time % first2last) + firstTime;
}

void cFrame::CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16 & matT)
{
	if (m_vecPosTrack.size() == 0) return;

	int count = 0;
	int n1 = 0;
	int n2 = 0;

	for (int i = 0; i < m_vecPosTrack.size(); i++)
	{
		if (n1 == 0) n1 = m_vecPosTrack[i].n;
		else if (n2 == 0) n2 = m_vecPosTrack[i].n;
		else
		{
			n1 = n2;
			n2 = m_vecPosTrack[i].n;
		}

		if (n1 <= nKeyFrame && nKeyFrame <= n2)
		{
			count = i;
			break;
		}
	}

	if (count == 0)
	{
		D3DXVECTOR3 vPos = m_vecPosTrack.back().v;
		D3DXMatrixTranslation(&matT, vPos.x, vPos.y, vPos.z);
	}
	else
	{
		float t = (nKeyFrame - n1) / float(n2 - n1);
		D3DXVECTOR3 vPos;
		D3DXVec3Lerp(&vPos, &m_vecPosTrack[count - 1].v, &m_vecPosTrack[count].v, t);

		D3DXMatrixTranslation(&matT, vPos.x, vPos.y, vPos.z);
	}
}

void cFrame::CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16 & matR)
{
	if (m_vecRotTrack.size() == 0) return;

	int count = 0;
	int n1 = 0;
	int n2 = 0;

	for (int i = 0; i < m_vecRotTrack.size(); i++)
	{
		if (n1 == 0) n1 = m_vecRotTrack[i].n;
		else if (n2 == 0) n2 = m_vecRotTrack[i].n;
		else
		{
			n1 = n2;
			n2 = m_vecRotTrack[i].n;
		}

		if (n1 <= nKeyFrame && nKeyFrame <= n2)
		{
			count = i;
			break;
		}
	}

	if (count == 0)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.back().q);
	}
	else
	{
		float t = (nKeyFrame - n1) / float(n2 - n1);

		D3DXQUATERNION qRot;
		D3DXQuaternionSlerp(&qRot, &m_vecRotTrack[count - 1].q, &m_vecRotTrack[count].q, t);

		D3DXMatrixRotationQuaternion(&matR, &qRot);
	}
}
