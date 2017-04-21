#pragma once
#include "cCubeNode.h"

class cLArm : public cCubeNode
{
public:
	cLArm();
	~cLArm();

	virtual void Setup() override;
	virtual void Update() override;
};

