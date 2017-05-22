#pragma once
#include "cCharacter.h"
#include "cFrame.h"
class cMtlTex;

class cWoman : public cCharacter
{
public:
	cWoman();
	~cWoman();

	virtual void Setup();
	virtual void Update(iMap* pMap);
	virtual void Render();

	//picking test
	void SetDestination(D3DXVECTOR3 vDestination);
	LPD3DXMESH GetMeshSphere() { return m_pMeshSphere; }
	void SetMtlSphere(bool isSelected)
	{
		if (isSelected)
		{
			m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
			m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
			m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
		}
	}

	//height map
	void ApplyHeightMap(std::vector<ST_PNT_VERTEX>& vecVertexHeightMap);

private:
	cFrame* m_pRootStand;
	cFrame* m_pRootRun;

	bool m_IsDestination;
	D3DXVECTOR3 m_vDestination;
	LPD3DXMESH m_pMeshSphere;
	D3DMATERIAL9 m_stMtlSphere;
};

