#pragma once

class cObject;

struct MtlTex
{
	D3DMATERIAL9 mtl;
	string texName;
};

class cObjLoader
{
public:
	cObjLoader();
	~cObjLoader();

	void LoadObj(const char* fileName);
	void LoadMaterial(string adress);
	void Render();

private:
	//LoadObj
	std::vector<D3DXVECTOR3> m_vecVertices;
	std::vector<D3DXVECTOR2> m_vecTextureVertices;
	std::vector<D3DXVECTOR3> m_vecVertexNormals;

	//LoadMaterial
	std::map<string, MtlTex> m_mapMaterial;

	//MakeObj
	string m_sObjName;
	string m_sMtlName;
	string m_sTextureName;
	std::vector<ST_PNT_VERTEX> m_vecVertex;

	//group
	std::vector<cObject> m_vecObj;
};