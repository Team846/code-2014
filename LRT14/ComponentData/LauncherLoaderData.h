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
	
	void ResetCommands();
	void Log();
	
private:
	bool m_fire;
};

#endif
