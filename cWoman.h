#pragma once
#include "cCharacter.h"
#include "cFrame.h"

class cWoman : public cCharacter
{
public:
	cWoman();
	~cWoman();

	virtual void Setup();
	virtual void Update(iMap* pMap);
	virtual void Render(bool useVertexBuffer, bool useIndexBuffer);

private:
	cFrame* m_pRootStand;
	cFrame* m_pRootRun;
};

