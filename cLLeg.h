#pragma once
#include "cCubeNode.h"

class cLLeg : public cCubeNode
{
public:
	cLLeg();
	~cLLeg();

	virtual void Setup() override;
	virtual void Update() override;
};

