#pragma once

#define g_pTimeManager cTimeManager::GetInstance() // << : 18

class cTimeManager
{
//public:
//	cTimeManager();
//	~cTimeManager();
	// >> : 18.
private:
	SINGLETONE(cTimeManager);

private:
	DWORD m_dwLastUpdateTime;
	float m_fEllapsedTime;

public:
	void Update();
	float GetElapsedTime();
	float GetLastUpdateTime();
	// << : 
};

