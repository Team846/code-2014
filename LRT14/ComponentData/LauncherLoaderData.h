#ifndef LAUNCHER_LOADER_DATA_H_
#define LAUNCHER_LOADER_DATA_H_

#include "ComponentData.h"

class LauncherLoaderData : public ComponentData
{
public:
	LauncherLoaderData();
	static LauncherLoaderData* Get();

	void ResetCommands();
	void Log();
};

#endif
