#pragma once

#define g_pTextureManager	cTextureManager::GetInstance()

class cTextureManager
{
public:
	SINGLETONE(cTextureManager);

private:
	map<string, LPDIRECT3DTEXTURE9> m_mTexture;

public:
	void AddTexture(string name, LPDIRECT3DTEXTURE9 texture);
	void DeleteTexture(string name, LPDIRECT3DTEXTURE9 texture);
	LPDIRECT3DTEXTURE9 FindTexture(string name);

	void Destroy();
};
