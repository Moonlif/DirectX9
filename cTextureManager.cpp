#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
	m_mTexture.clear();
}


cTextureManager::~cTextureManager()
{
}


void cTextureManager::AddTexture(string name, LPDIRECT3DTEXTURE9 texture)
{
	if (FindTexture(name) == NULL)
		m_mTexture.insert(make_pair(name, texture));
}

void cTextureManager::DeleteTexture(string name, LPDIRECT3DTEXTURE9 texture)
{
	if (FindTexture(name) != NULL)
		m_mTexture.erase(name);
}

LPDIRECT3DTEXTURE9 cTextureManager::FindTexture(string name)
{
	map<string, LPDIRECT3DTEXTURE9>::iterator iter;
	iter = m_mTexture.find(name);
	if (iter != m_mTexture.end())
	{
		return iter->second;
	}
	
	return NULL;
}

void cTextureManager::Destroy()
{
	map<string, LPDIRECT3DTEXTURE9>::iterator iter;
	for (iter = m_mTexture.begin(); iter != m_mTexture.end(); ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
	m_mTexture.clear();
}
