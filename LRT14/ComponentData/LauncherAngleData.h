#ifndef LAUNCHER_ANGLE_DATA_H_
#define LAUNCHER_ANGLE_DATA_H_

#include "ComponentData.h"

class LauncherAngleData : public ComponentData
{
public:
	enum Angle
	{
		LOW,
		HIGH
	};
	
	LauncherAngleData();
	static LauncherAngleData* Get();
	
	Angle GetAngle();
	void SetAngle(Angle angle);

	void ResetCommands();
	void Log();
	
private:
	Angle m_launcherAngle;
};

#endif
