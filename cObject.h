#pragma once
#include "cMtlTex.h"

class cObject : public cMtlTex
{
public:
	cObject();
	~cObject();

private:
	string						m_sObjName;
	std::vector<ST_PNT_VERTEX>	m_vecVertex;
	
	D3DXVECTOR3					m_vScale;

public:
	void Setup(string name, D3DMATERIAL9 material, string textureName, vector<ST_PNT_VERTEX> vertex, D3DXVECTOR3 scale);
	virtual void Render() override;
	void Destroy();
};

