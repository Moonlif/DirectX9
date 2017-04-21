#pragma once
#include "cCubeNode.h"

class cRArm : public cCubeNode
{
public:
	cRArm();
	~cRArm();

	virtual void Setup() override;
	virtual void Update() override;
};

