#pragma once
#include "cMtlTex.h"

class cGeomObject
{
public:
	cGeomObject();
	~cGeomObject();

private:
	SYNTHESIZE(string, m_sName, Name);
	SYNTHESIZE(string, m_sParentName, ParentName);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, matWorld);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	void Render();
};

//cFrame으로 대체함