#pragma once
class cRay
{
public:
	cRay();
	~cRay();

	enum eRaySpace
	{
		E_NONE,
		E_VIEW,
		E_WORLD,
		E_LOCAL
	};

protected:
	D3DXVECTOR3 m_vOrigin;
	D3DXVECTOR3 m_vDirection;
	eRaySpace m_eRaySpace;

public:
	static cRay RayAtViewSpace(int nScreenX, int nScreenY);
	static cRay RayAtWorldSpace(int nScreenX, int nScreenY);

	bool IntersectTri(IN D3DXVECTOR3& v0, IN D3DXVECTOR3& v1, IN D3DXVECTOR3& v2, OUT D3DXVECTOR3& vPickedPosition);
	bool IsPicked(ST_SPHERE* pSphere);

};

