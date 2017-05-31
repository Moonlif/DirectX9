#pragma once
class cFrustum
{
public:
	cFrustum();
	~cFrustum();

private:
	std::vector<D3DXVECTOR3>	m_vecProjVertex;
	std::vector<D3DXVECTOR3>	m_vecWorldVertex;
	std::vector<D3DXPLANE>		m_vecPlane;

public:
	void Setup();
	void Update();
	bool IsIn(D3DXVECTOR3* vPosition);

};

