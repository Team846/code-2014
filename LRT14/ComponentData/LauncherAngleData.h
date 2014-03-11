#ifndef LAUNCHER_ANGLE_DATA_H_
#define LAUNCHER_ANGLE_DATA_H_

#include "ComponentData.h"

/*!
 * @brief Contains commands and states for the launcher angle component.
 */
class LauncherAngleData : public ComponentData
{
public:
	enum Angle
	{
		SHORT,
		LONG
	};
	
	LauncherAngleData();
	static LauncherAngleData* Get();
	
	Angle GetAngle();
	void SetAngle(Angle angle);
	
	void ResetCommands();
	void Log();
	
private:
	Angle m_launcherAngle;
	
	friend class LauncherAngle;
};

#endif
