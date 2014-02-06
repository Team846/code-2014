#ifndef CHANGE_LAUNCHER_ANGLE_H_
#define CHANGE_LAUNCHER_ANGLE_H_

#include "Automation.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/LauncherAngleData.h"

/*!
 * @brief Changes the launch angle of the launcher.
 */
class ChangeLauncherAngle : public Automation
{
public:
	ChangeLauncherAngle(bool longShot);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	LauncherLoaderData* m_loaderData;
	LauncherAngleData* m_angleData;
	
	bool m_long;
};

#endif
