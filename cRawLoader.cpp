#include "stdafx.h"
#include "cRawLoader.h"


cRawLoader::cRawLoader()
{
}


cRawLoader::~cRawLoader()
{
}

void cRawLoader::Load(OUT std::vector<unsigned int>& vecHeightMap, IN char * szFullPath)
{
	FILE *fp;
	fopen_s(&fp, szFullPath, "rb");

	while (true)
	{
		if (feof(fp)) break;

		unsigned int temp = fgetc(fp);
		vecHeightMap.push_back(temp);
	}
}
