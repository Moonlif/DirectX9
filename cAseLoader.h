#pragma once

class cMtlTex;
class cGeomObject;

class cAseLoader
{
public:
	cAseLoader();
	~cAseLoader();

private:
	std::vector<cMtlTex*> m_vecMtlTex;

public:
	void Load(OUT std::vector<cGeomObject*> & vecGeomObject, IN char* szFolder, IN char* szFile);

};

