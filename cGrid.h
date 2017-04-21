#pragma once
class cGrid
{
public:
	cGrid();
	~cGrid();

private:
	std::vector<ST_PC_VERTEX>		m_vecVertex;

public:
	void Setup(int xAxisCount, int zAxisCount, float interval);
	void Render();

};

