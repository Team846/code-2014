#ifndef LAUNCHER_LOADER_DATA_H_
#define LAUNCHER_LOADER_DATA_H_

#include "ComponentData.h"

class LauncherLoaderData : public ComponentData
{
public:
	LauncherLoaderData();
	static LauncherLoaderData* Get();

	void SetFire(bool shouldFire);
	bool GetFire();
	void SetPurge(bool purge);
	bool GetPurge();
	void SetLoad(bool load);
	bool GetLoad();

	bool IsLoadingComplete();
	int GetSensorValue();
	
	void ResetCommands();
	void Log();
	
private:
	void SetLoadingComplete(bool complete);
	void SetSensorValue(int value);
	
	bool m_fire;
	bool m_purge;
	bool m_load;
	bool m_complete;
	int m_value;
	
	friend class LauncherLoader;
};

#endif
