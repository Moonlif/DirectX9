#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cFrame : public cObject
{
public:
	cFrame();
	~cFrame();

private:
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocalTM, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

	std::vector<cFrame*> m_vecChild;

	int						m_nNumTri;
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;

	SYNTHESIZE_ADD_REF(LPD3DXMESH, m_pMesh, Mesh);

public:
	void Update(int nKeyFrame, D3DXMATRIXA16* pmatParent);
	void Render();
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcOriginalLocalTM(D3DXMATRIXA16* pmatParent);

	DWORD	m_dwFirstFrame;
	DWORD	m_dwLastFrame;
	DWORD	m_dwFrameSpeed;
	DWORD	m_dwTicksPerFrame;
	DWORD	m_dwPreTime;

	int GetKeyFrame();
	void CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16& matT);
	void CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16& matR);

	void BuildVertexBuffer(std::vector<ST_PNT_VERTEX>& vecVertex);
	void BuildMesh(std::vector<ST_PNT_VERTEX>& vecVertex);
};

