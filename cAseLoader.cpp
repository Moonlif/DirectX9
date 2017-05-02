#include "stdafx.h"
#include "cAseLoader.h"

#include "Asciitok.h"
#include "cGeomObject.h"

cAseLoader::cAseLoader()
{
}


cAseLoader::~cAseLoader()
{
}

void cAseLoader::Load(OUT std::vector<cGeomObject*>& vecGeomObject, IN char * szFolder, IN char * szFile)
{
	//Scene
	//string scene_Name;
	//int scene_FirstFrame, scene_LastFrame, sceneFrameSpeed;

	//Material
	m_vecMtlTex.clear();

	//GeomObject
	std::string sObjName;
	std::string sObjParentName;
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	std::vector<ST_PNT_VERTEX> vecV;
	std::vector<int> vecFaceIndex;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;
	int nMtlTexIndex = 0;
	int nMtlNum = 0;

	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	FILE *fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	while (true)
	{
		if (feof(fp))
		{
			if (vecFaceIndex.empty() == false)
			{
				for (int i = 0; i < vecFaceIndex.size(); i += 3)
				{
					vecVertex[i].p = vecV[vecFaceIndex[i]].p;
					vecVertex[i].n = vecV[vecFaceIndex[i]].n;
				}

				cGeomObject* pGeomObj = new cGeomObject;
				pGeomObj->SetName(sObjName);
				pGeomObj->SetParentName(sObjParentName);
				pGeomObj->SetmatWorld(matWorld);
				pGeomObj->SetVertex(vecVertex);
				pGeomObj->SetMtlTex(m_vecMtlTex[nMtlNum]);
				vecGeomObject.push_back(pGeomObj);
			}
			break;
		}

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		char szTypeName[1024];
		sscanf_s(szTemp, "%s", szTypeName, 1024);
		string sTypeName = szTypeName;

		if (sTypeName == ID_SCENE)
		{
			continue;
		}
		else if (sTypeName == ID_MATERIAL_COUNT)
		{
			int Material_Count;
			sscanf_s(szTemp, "%*s %d", &Material_Count);
			m_vecMtlTex.resize(Material_Count);
			
			for (int i = 0; i < Material_Count; ++i)
			{
				m_vecMtlTex[i] = new cMtlTex;
			}
		}
		else if (sTypeName == ID_MATERIAL)
		{
			sscanf_s(szTemp, "%*s %d", &nMtlTexIndex);
		}
		else if (sTypeName == ID_AMBIENT)
		{
			float r, g, b;
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Ambient.r = r;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Ambient.g = g;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Ambient.b = b;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Ambient.a = 1.0f;
		}
		else if (sTypeName == ID_DIFFUSE)
		{
			float r, g, b;
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Diffuse.r = r;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Diffuse.g = g;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Diffuse.b = b;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Diffuse.a = 1.0f;
		}
		else if (sTypeName == ID_SPECULAR)
		{
			float r, g, b;
			sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Specular.r = r;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Specular.g = g;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Specular.b = b;
			m_vecMtlTex[nMtlTexIndex]->GetMaterial().Specular.a = 1.0f;
		}
		else if (sTypeName == ID_BITMAP)
		{
			char szTexFile[1024];
			sscanf_s(szTemp, "%*s %s", szTexFile, 1024);
			string sTexFile = szTexFile;
			sTexFile.erase(0, 9);
			string sTexFullPath = szFolder;
			sTexFullPath += (std::string("/") + sTexFile);

			LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(sFullPath);
			m_vecMtlTex[nMtlNum]->SetTexture(pTexture);
		}

		else if (sTypeName == ID_GEOMETRY)
		{
			if (vecFaceIndex.empty() == false)
			{
				for (int i = 0; i < vecFaceIndex.size(); i += 3)
				{
					vecVertex[i].p = vecV[vecFaceIndex[i]].p;
					vecVertex[i].n = vecV[vecFaceIndex[i]].n;
				}

				cGeomObject* pGeomObj = new cGeomObject;
				pGeomObj->SetName(sObjName);
				pGeomObj->SetParentName(sObjParentName);
				pGeomObj->SetmatWorld(matWorld);
				pGeomObj->SetVertex(vecVertex);
				pGeomObj->SetMtlTex(m_vecMtlTex[nMtlNum]);
				vecGeomObject.push_back(pGeomObj);
			}

			sObjName.clear();
			sObjParentName.clear();
			D3DXMatrixIdentity(&matWorld);
			vecVertex.clear();
			vecV.clear();
			vecFaceIndex.clear();
			vecVT.clear();
			nMtlNum = 0;
		}
		else if (sTypeName == ID_NODE_NAME)
		{
			char szObjName[1024];
			sscanf_s(szTemp, "%*s %s", szObjName, 1024);
			sObjName = szObjName;
		}
		else if (sTypeName == ID_NODE_PARENT)
		{
			char szObjParentName[1024];
			sscanf_s(szTemp, "%*s %s", szObjParentName, 1024);
			sObjParentName = szObjParentName;
		}
		else if (sTypeName == ID_TM_ROW0)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[0] = x;
			matWorld[1] = y;
			matWorld[2] = z;
		}
		else if (sTypeName == ID_TM_ROW1)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[4] = x;
			matWorld[5] = y;
			matWorld[6] = z;
		}
		else if (sTypeName == ID_TM_ROW2)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[8] = x;
			matWorld[9] = y;
			matWorld[10] = z;
		}
		else if (sTypeName == ID_TM_ROW3)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[12] = x;
			matWorld[13] = y;
			matWorld[14] = z;
		}


		else if (sTypeName == ID_MESH_NUMVERTEX)
		{
			int Mesh_NumVertex = 0;
			sscanf_s(szTemp, "%*s %d", &Mesh_NumVertex);
			vecV.resize(Mesh_NumVertex);
		}
		else if (sTypeName == ID_MESH_NUMFACES)
		{
			int Mesh_NumFaces = 0;
			sscanf_s(szTemp, "%*s %d", &Mesh_NumFaces);
			vecVertex.resize(Mesh_NumFaces * 3);
		}
		else if (sTypeName == ID_MESH_VERTEX)
		{
			int nVertexIndex;
			float x, y, z;
			sscanf_s(szTemp, "%*s %d %f %f %f", &nVertexIndex, &x, &y, &z);
			vecV[nVertexIndex].p.x = x;
			vecV[nVertexIndex].p.y = y;
			vecV[nVertexIndex].p.z = -z;
		}
		else if (sTypeName == ID_MESH_FACE)
		{
			int nFaceIndex, p0, p1, p2;
			sscanf_s(szTemp, "%*s %d %*c %*s %d %*s %d %*s %d", &nFaceIndex, &p0, &p1, &p2);
			vecFaceIndex.push_back(p0);
			vecFaceIndex.push_back(p1);
			vecFaceIndex.push_back(p2);
		}
		else if (sTypeName == ID_MESH_NUMTVERTEX)
		{
			int Mesh_NumTVertex = 0;
			sscanf_s(szTemp, "%*s %d", &Mesh_NumTVertex);
			vecVT.resize(Mesh_NumTVertex);
		}
		else if (sTypeName == ID_MESH_TVERT)
		{
			int nTVertexIndex;
			float u, v;
			sscanf_s(szTemp, "%*s %d %f %f %*f", &nTVertexIndex, &u, &v);
			vecVT[nTVertexIndex] = D3DXVECTOR2(u, v);
		}
		else if (sTypeName == ID_MESH_TFACE)
		{
			int nTFaceIndex, p0, p1, p2;
			sscanf_s(szTemp, "%*s %d %d %d %d", &nTFaceIndex, &p0, &p1, &p2);
			vecVertex[nTFaceIndex * 3 + 0].t = vecVT[p0];
			vecVertex[nTFaceIndex * 3 + 1].t = vecVT[p1];
			vecVertex[nTFaceIndex * 3 + 2].t = vecVT[p2];
		}
		else if (sTypeName == ID_MESH_VERTEXNORMAL)
		{
			int nNFaceIndex;
			float x, y, z;
			sscanf_s(szTemp, "%*s %d %f %f %f", &nNFaceIndex, &x, &y, &z);
			vecV[nNFaceIndex].n = D3DXVECTOR3(x, y, z);
		}
		else if (sTypeName == ID_MATERIAL_REF)
		{
			sscanf_s(szTemp, "%*s %d", &nMtlNum);
		}
		else continue;
	}

	fclose(fp);

	for each(auto i in m_vecMtlTex)
	{
		SAFE_RELEASE(i);
	}
	m_vecMtlTex.clear();
}
