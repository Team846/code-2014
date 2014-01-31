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

	bool IsLoadingComplete();
	
	void ResetCommands();
	void Log();
	
private:
	void SetLoadingComplete(bool complete);
	
	bool m_fire;
	bool m_complete;
	
	friend class LauncherLoader;
};

#endif
