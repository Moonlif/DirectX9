#pragma once
class cGrid
{
public:
	cGrid();
	~cGrid();

private:
	std::vector<ST_PC_VERTEX>		m_vecVertex;
	int						m_nNumLine;
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;

public:
	void Setup(int xAxisStart, int zAxisStart, int xAxisCount, int zAxisCount, float interval);
	void Render();

};

