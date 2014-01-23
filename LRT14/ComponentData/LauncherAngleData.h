#ifndef LAUNCHER_ANGLE_DATA_H_
#define LAUNCHER_ANGLE_DATA_H_

#include "ComponentData.h"

class LauncherAngleData : public ComponentData
{
public:
	LauncherAngleData();
	static LauncherAngleData* Get();

	void ResetCommands();
	void Log();
};

#endif
