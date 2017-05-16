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
	virtual void Update(iMap* pMap);
	virtual void Render();

private:
	cFrame* m_pRootStand;
	cFrame* m_pRootRun;
};

