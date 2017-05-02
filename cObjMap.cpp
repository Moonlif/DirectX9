#include "stdafx.h"
#include "cObjMap.h"
#include "cObjLoader.h"

cObjMap::cObjMap(IN char* szFolder, IN char* szFile, IN D3DXMATRIXA16* pMat)
{
	Load(szFolder, szFile, pMat);
}


cObjMap::~cObjMap()
{
}

void cObjMap::Load(IN char * szFolder, IN char * szFile, IN D3DXMATRIXA16 * pMat)
{
	cObjLoader loadObj;
	loadObj.LoadSurfcae(m_vecSurface, szFolder, szFile, pMat);
}

bool cObjMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	D3DXVECTOR3 vRayPos(x, 1000, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		float u, v, f;

		if (D3DXIntersectTri(
			&m_vecSurface[i + 0],
			&m_vecSurface[i + 1],
			&m_vecSurface[i + 2],
			&vRayPos,
			&vRayDir,
			&u,
			&v,
			&f))
		{
			y = 1000 - f;
			return true;
		}
	}
	return false;
}

