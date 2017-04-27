#include "stdafx.h"
#include "cObjLoader.h"


cObjLoader::cObjLoader()
{
	m_vVertices;
	m_vTextureVertices;
	m_vVertexNormals;
	m_vecVertex;
}


cObjLoader::~cObjLoader()
{
}

void cObjLoader::LoadObj()
{
	FILE* fp = NULL;
	fopen_s(&fp, "objects/box.obj", "r");
	char szTemp[1024];
	float x, y, z;

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
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			D3DXVECTOR3 vecTemp(x, y, z);
			m_vVertices.push_back(vecTemp);
		}
		else if (type == "vt")
		{
			sscanf_s(szTemp, "%*s %f %f", &x, &y);
			D3DXVECTOR2 vecTemp(x, y);
			m_vTextureVertices.push_back(vecTemp);
		}
		else if (type == "vn")
		{
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			D3DXVECTOR3 vecTemp(x, y, z);
			m_vVertexNormals.push_back(vecTemp);
		}

		else if (type == "f")
		{
			int p1, t1, n1, p2, t2, n2, p3, t3, n3;

			sscanf_s(szTemp, "%*s   %d %*c %d %*c %d	%d %*c %d %*c %d	%d %*c %d %*c %d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);

			ST_PNT_VERTEX tempVertex;
			tempVertex.p = m_vVertices[p1 - 1];
			tempVertex.t = m_vTextureVertices[t1 - 1];
			tempVertex.n = m_vVertexNormals[n1 - 1];
			m_vecVertex.push_back(tempVertex);

			tempVertex.p = m_vVertices[p2 - 1];
			tempVertex.t = m_vTextureVertices[t2 - 1];
			tempVertex.n = m_vVertexNormals[n2 - 1];
			m_vecVertex.push_back(tempVertex);

			tempVertex.p = m_vVertices[p3 - 1];
			tempVertex.t = m_vTextureVertices[t3 - 1];
			tempVertex.n = m_vVertexNormals[n3 - 1];
			m_vecVertex.push_back(tempVertex);
		}
	}
}

void cObjLoader::LoadMaterial(string adress)
{
	FILE* fp = NULL;
	fopen_s(&fp, adress.c_str(), "r");
	char szTemp[1024];
	float r, g, b;

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));

	while (fp != NULL)
	{
		if (feof(fp)) break;
		fgets(szTemp, 1024, fp);
		char typeTemp[128];
		sscanf_s(szTemp, "%s", &typeTemp, 128);
		string type = typeTemp;

		if (type == "#") continue;
		else if (type == "Ka")
		{
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			m_stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (type == "Kd")
		{
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			m_stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (type == "Ks")
		{
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			m_stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (type == "d")
		{
			sscanf_s(szTemp, "%*s %f", &r);
			m_stMtl.Power = r;
		}
		else if (type == "map_Kd")
		{
			char textureAdress[128];
			sscanf_s(szTemp, "%*s %s", &textureAdress, 128);
			string sTextureAdress = textureAdress;
			string adress = "objects/" + sTextureAdress;
			LoadTexture(adress);
		}
	}
}

void cObjLoader::LoadTexture(string adress)
{
	D3DXCreateTextureFromFile(g_pD3DDevice, adress.c_str(), &m_pTexture);
}

void cObjLoader::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetTexture(0, m_pTexture);

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));

	g_pD3DDevice->SetTexture(0, NULL);
}
