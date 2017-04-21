#pragma once
#include "cCubeNode.h"

class cRLeg : public cCubeNode
{
public:
	cRLeg();
	~cRLeg();

	virtual void Setup() override;
	virtual void Update() override;
};

