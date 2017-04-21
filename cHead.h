#pragma once
#include "cCubeNode.h"

class cHead : public cCubeNode
{
public:
	cHead();
	~cHead();

	virtual void Setup() override;
	virtual void Update() override;

	bool m_Isfront;
};

