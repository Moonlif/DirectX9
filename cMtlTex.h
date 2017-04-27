#pragma once

class cMtlTex
{
public:
	cMtlTex();
	~cMtlTex();

protected:
	D3DMATERIAL9 m_stMtl;
	string	m_sTextureName;

public:
	virtual void Render();
};

