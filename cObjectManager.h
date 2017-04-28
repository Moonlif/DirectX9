#pragma once
#define g_ObjectManager cObjectManager::GetInstance()

class cObjectManager
{
private:
	SINGLETONE(cObjectManager);

	std::map<std::string, cObject> m_mapObject;

public:
	void RemoveObject(cObject* obj);
	void Destroy();
};

