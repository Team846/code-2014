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
	
	bool IsCompleteState();
	
	void ResetCommands();
	void Log();
	
private:
	void SetCompleteState(bool complete);
	
	Angle m_launcherAngle;
	bool m_complete;
	
	friend class LauncherAngle;
};

#endif
