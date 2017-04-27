#include "stdafx.h"
#include "cObjLoader.h"
#include "cObject.h"
#include "cMtlTex.h"

cObjLoader::cObjLoader()
{
}


cObjLoader::~cObjLoader()
{
}

void cObjLoader::LoadObj(const char* fileName)
{
	FILE* fp = NULL;
	fopen_s(&fp, fileName, "r");
	char szTemp[1024];

	while (fp != NULL)
	{
		if (feof(fp)) break;
		fgets(szTemp, 1024, fp);
		char typeTemp[128];
		sscanf_s(szTemp, "%s", &typeTemp, 128);
		string type = typeTemp;

		if (type == "#") continue;
		else if (type == "mtllib")
		{
			char mtlAdress[128];
			sscanf_s(szTemp, "%*s %s", &mtlAdress, 128);
			string sMtlAddress = mtlAdress;
			string adress = "objects/" + sMtlAddress;
			LoadMaterial(adress);
		}
		else if (type == "v")
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			D3DXVECTOR3 vecTemp(x, y, z);
			m_vecVertices.push_back(vecTemp);
		}
		else if (type == "vt")
		{
			float u, v;
			sscanf_s(szTemp, "%*s %f %f", &u, &v);
			D3DXVECTOR2 vecTemp(u, v);
			m_vecTextureVertices.push_back(vecTemp);
		}
		else if (type == "vn")
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			D3DXVECTOR3 vecTemp(x, y, z);
			m_vecVertexNormals.push_back(vecTemp);
		}

		//make faces
		else if (type == "f")
		{
			int p1, t1, n1, p2, t2, n2, p3, t3, n3;

			sscanf_s(szTemp, "%*s   %d %*c %d %*c %d	%d %*c %d %*c %d	%d %*c %d %*c %d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);

			ST_PNT_VERTEX tempVertex;
			tempVertex.p = m_vecVertices[p1 - 1];
			tempVertex.t = m_vecTextureVertices[t1 - 1];
			tempVertex.n = m_vecVertexNormals[n1 - 1];
			m_vecVertex.push_back(tempVertex);

			tempVertex.p = m_vecVertices[p2 - 1];
			tempVertex.t = m_vecTextureVertices[t2 - 1];
			tempVertex.n = m_vecVertexNormals[n2 - 1];
			m_vecVertex.push_back(tempVertex);

			tempVertex.p = m_vecVertices[p3 - 1];
			tempVertex.t = m_vecTextureVertices[t3 - 1];
			tempVertex.n = m_vecVertexNormals[n3 - 1];
			m_vecVertex.push_back(tempVertex);
		}

		//make object
		else if (type == "g")
		{
			if (m_vecVertex.size() != 0)
			{
				cObject tempObj;

				map<string, MtlTex>::iterator miter;
				miter = m_mapMaterial.find(m_sMtlName);
				if (miter != m_mapMaterial.end())
				{
					D3DMATERIAL9 tempMtl = miter->second.mtl;
					m_sTextureName = miter->second.texName;

					tempObj.Setup(m_sObjName, tempMtl, m_sTextureName, m_vecVertex, D3DXVECTOR3(0.01f, 0.01f, 0.01f));
					m_vecObj.push_back(tempObj);
				}
			}
			m_vecVertex.clear();

			char objName[128];
			sscanf_s(szTemp, "%*s %s", &objName, 128);
			m_sObjName = objName;
		}

		//select mtl
		else if (type == "usemtl")
		{
			char mtlName[128];
			sscanf_s(szTemp, "%*s %s", &mtlName, 128);
			m_sMtlName = mtlName;
		}
	}
}

void cObjLoader::LoadMaterial(string adress)
{
	FILE* fp = NULL;
	fopen_s(&fp, adress.c_str(), "r");
	char szTemp[1024];

	MtlTex tempMtlTex;
	ZeroMemory(&tempMtlTex, sizeof(MtlTex));
	string tempMtlName;

	while (fp != NULL)
	{
		if (feof(fp)) break;
		fgets(szTemp, 1024, fp);
		char typeTemp[128];
		sscanf_s(szTemp, "%s", &typeTemp, 128);
		string type = typeTemp;

		if (type == "#") continue;
		else if (type == "newmtl")
		{
			char mtlName[128];
			sscanf_s(szTemp, "%*s %s", &mtlName, 128);
			tempMtlName = mtlName;
		}
		else if (type == "Ka")
		{
			float r, g, b;
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			tempMtlTex.mtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (type == "Kd")
		{
			float r, g, b;
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			tempMtlTex.mtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (type == "Ks")
		{
			float r, g, b;
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			tempMtlTex.mtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (type == "d")
		{
			float d;
			sscanf_s(szTemp, "%*s %f", &d);
			tempMtlTex.mtl.Power = d;
		}
		else if (type == "map_Kd")
		{
			char textureName[128];
			sscanf_s(szTemp, "%*s %s", &textureName, 128);
			string sTextureName = textureName;
			string adress = "objects/" + sTextureName;

			tempMtlTex.texName = sTextureName;
			LPDIRECT3DTEXTURE9 tempTexture = NULL;
			D3DXCreateTextureFromFile(g_pD3DDevice, adress.c_str(), &tempTexture);
			if (tempTexture != NULL) g_pTextureManager->AddTexture(sTextureName, tempTexture);

			//add material to map(m_mapMaterial)
			m_mapMaterial.insert(make_pair(tempMtlName, tempMtlTex));
			
			//reset the tempMtlTex
			ZeroMemory(&tempMtlTex, sizeof(MtlTex));
		}
	}
}

void cObjLoader::Render()
{
	for (int i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i].Render();
	}
}
