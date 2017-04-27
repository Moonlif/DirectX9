#pragma once

class cObjLoader
{
public:
	cObjLoader();
	~cObjLoader();

	void LoadObj();
	void LoadMaterial(string adress);
	void LoadTexture(string adress);
	void Render();

private:
	std::vector<D3DXVECTOR3> m_vVertices;
	std::vector<D3DXVECTOR2> m_vTextureVertices;
	std::vector<D3DXVECTOR3> m_vVertexNormals;

	std::vector<ST_PNT_VERTEX> m_vecVertex;

	D3DMATERIAL9 m_stMtl;
	LPDIRECT3DTEXTURE9 m_pTexture;
};

