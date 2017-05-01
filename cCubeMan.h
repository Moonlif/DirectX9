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
	LPDIRECT3DTEXTURE9 m_pTexture;

	virtual void Setup(bool isLerp);
	virtual void Update() override;
	virtual void Render() override;

	void Set_Material();
	void MoveToDest();

private:
	//bezier curve
	bool m_IsLerp;
	int m_nDestIndex;
	D3DXVECTOR3 m_vDest[6];
	float m_fDt;
};

