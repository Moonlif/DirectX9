#pragma once
#include "cCharacter.h"
#include "cFrame.h"
class cMtlTex;

class cWoman : public cCharacter
{
public:
	cWoman();
	~cWoman();

	virtual void Setup();
	virtual void Update(iMap* pMap, std::vector<ST_PNT_VERTEX>& vecVertexPlane);
	virtual void Render();

private:
	cFrame* m_pRootStand;
	cFrame* m_pRootRun;

	bool m_IsDestination;
	D3DXVECTOR3 m_vDestination;
};

