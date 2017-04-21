#pragma once
#include "cCharacter.h"
class cCubeNode;

class cCubeMan : public cCharacter
{
public:
	cCubeMan();
	~cCubeMan();

public:
	cCubeNode* m_pRoot;
	D3DMATERIAL9	m_stMtl;

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};

