#pragma once
class cRawLoader
{
public:
	cRawLoader();
	~cRawLoader();

public:
	void Load(OUT std::vector<unsigned int>& vecHeightMap, IN char* szFullPath);

};

