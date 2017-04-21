#pragma once
class cPyramid
{
public:
	cPyramid();
	~cPyramid();

private:
	std::vector<ST_PC_VERTEX>		m_vecVertex;

public:
	void Setup();
	void Render();

};

