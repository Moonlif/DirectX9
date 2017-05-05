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
	int count = 0;
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
	std::vector<int> vecFaceIndex;
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<D3DXVECTOR3> vecVN;
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
				for (int i = 0; i < vecFaceIndex.size(); ++i)
				{
					vecVertex[i].p = vecV[vecFaceIndex[i]];
					vecVertex[i].n = vecVN[vecFaceIndex[i]];
				}

				if (sObjParentName.empty() == false)
				{
					for each (auto it in vecGeomObject)
					{
						if (it->GetName() == sObjParentName)
						{
							D3DXMATRIXA16 tempMat;
							D3DXMatrixInverse(&tempMat, NULL, &it->GetmatWorld());
							matWorld = matWorld * tempMat;
						}
					}
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

			vecFaceIndex.clear();
			vecV.clear();
			vecVT.clear();
			vecVN.clear();
			vecVertex.clear();
			nMtlNum = 0;

			break;
		}

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		char szTypeName[1024];
		sscanf_s(szTemp, "%s", szTypeName, 1024);
		string sTypeName = szTypeName;

		if (sTypeName == "}") continue;

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
			sTexFile.pop_back();
			string sTexFullPath = szFolder;
			sTexFullPath += (std::string("/") + sTexFile);

			LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(sTexFullPath);
			m_vecMtlTex[nMtlTexIndex]->SetTexture(pTexture);
		}

		else if (sTypeName == ID_GEOMETRY)
		{
			if (vecFaceIndex.empty() == false)
			{
				for (int i = 0; i < vecFaceIndex.size(); ++i)
				{
					vecVertex[i].p = vecV[vecFaceIndex[i]];
					vecVertex[i].n = vecVN[vecFaceIndex[i]];
				}

				if (sObjParentName.empty() == false)
				{
					for each (auto it in vecGeomObject)
					{
						if (it->GetName() == sObjParentName)
						{
							D3DXMATRIXA16 tempMat;
							D3DXMatrixInverse(&tempMat, NULL, &it->GetmatWorld());
							matWorld = matWorld * tempMat;
						}
					}
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

			vecFaceIndex.clear();
			vecV.clear();
			vecVT.clear();
			vecVN.clear();
			vecVertex.clear();
			nMtlNum = 0;
		}
		else if (sTypeName == ID_NODE_NAME)
		{
			sObjName = std::string(szTemp).erase(0, sTypeName.size() + 4);
			sObjName.pop_back();
			sObjName.pop_back();
		}
		else if (sTypeName == ID_NODE_PARENT)
		{
			sObjParentName = std::string(szTemp).erase(0, sTypeName.size() + 3);
			sObjParentName.pop_back();
			sObjParentName.pop_back();
		}
		else if (sTypeName == ID_TM_ROW0)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[0] = x;
			matWorld[2] = y;
			matWorld[1] = z;
		}
		else if (sTypeName == ID_TM_ROW1)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[8] = x;
			matWorld[10] = y;
			matWorld[9] = z;
		}
		else if (sTypeName == ID_TM_ROW2)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[4] = x;
			matWorld[6] = y;
			matWorld[5] = z;
		}
		else if (sTypeName == ID_TM_ROW3)
		{
			float x, y, z;
			sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
			matWorld[12] = x;
			matWorld[14] = y;
			matWorld[13] = z;
		}

		else if (sTypeName == ID_MESH_NUMVERTEX)
		{
			int Mesh_NumVertex = 0;
			sscanf_s(szTemp, "%*s %d", &Mesh_NumVertex);
			vecV.resize(Mesh_NumVertex);
			vecVN.resize(Mesh_NumVertex);
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
			vecV[nVertexIndex].x = x;
			vecV[nVertexIndex].y = z;
			vecV[nVertexIndex].z = y;
		}
		else if (sTypeName == ID_MESH_FACE)
		{
			int nFaceIndex, p0, p1, p2;
			sscanf_s(szTemp, "%*s %d %*c %*s %d %*s %d %*s %d", &nFaceIndex, &p0, &p1, &p2);
			vecFaceIndex.push_back(p0);
			vecFaceIndex.push_back(p2);
			vecFaceIndex.push_back(p1);
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
			float u, v, w;
			sscanf_s(szTemp, "%*s %d %f %f %f", &nTVertexIndex, &u, &v, &w);
			vecVT[nTVertexIndex] = D3DXVECTOR2(u, 1.0 - v);
		}
		else if (sTypeName == ID_MESH_TFACE)
		{
			int nTFaceIndex, p0, p1, p2;
			sscanf_s(szTemp, "%*s %d %d %d %d", &nTFaceIndex, &p0, &p1, &p2);
			vecVertex[nTFaceIndex * 3 + 0].t = vecVT[p0];
			vecVertex[nTFaceIndex * 3 + 1].t = vecVT[p2];
			vecVertex[nTFaceIndex * 3 + 2].t = vecVT[p1];
		}
		else if (sTypeName == ID_MESH_VERTEXNORMAL)
		{
			int nNFaceIndex;
			float x, y, z;
			sscanf_s(szTemp, "%*s %d %f %f %f", &nNFaceIndex, &x, &y, &z);
			vecVN[nNFaceIndex] = D3DXVECTOR3(x, z, y);
		}
		else if (sTypeName == ID_MATERIAL_REF)
		{
			sscanf_s(szTemp, "%*s %d", &nMtlNum);
		}
	}

	fclose(fp);

	for each(auto i in m_vecMtlTex)
	{
		SAFE_RELEASE(i);
	}
	m_vecMtlTex.clear();
}