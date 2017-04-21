#pragma once
#include "cCubePNT.h"

class cCubeNode : public cCubePNT
{
public:
	cCubeNode();

protected:
	D3DXVECTOR3		m_vLocalPos;
	D3DXMATRIXA16	m_matLocalTM;
	D3DXMATRIXA16	m_matWorldTM;
	std::vector<cCubeNode*> m_vecChild;

	SYNTHESIZE(D3DXMATRIXA16*, m_pParentWorldTM, ParentWorldTM);
	SYNTHESIZE(float, m_fRotDeltaX, RotDeltaX);
	SYNTHESIZE(float, m_fRotDeltaZ, RotDeltaZ);
	SYNTHESIZE(bool*, m_IsMoving, IsMoving);
	SYNTHESIZE(bool*, m_IsJumping, IsJumping);

	bool m_Isfront;

public:
	virtual ~cCubeNode();

	virtual void AddChild(cCubeNode* pChild);
	virtual void Destroy();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Swing();
};

