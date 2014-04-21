#ifndef PURGE_LAUNCHER_H_
#define PURGE_LAUNCHER_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/BallHolderData.h"
#include "../../ComponentData/BallPusherData.h"
#include "../../Config/Configurable.h"
#include "../../Sensors/SensorFactory.h"
#include <Timer.h>

/*!
 * @brief Moves the ball from the launcher to the collector using the hoop.
 */
class PurgeLauncher : public Automation, public Configurable
{
public:
	PurgeLauncher();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	CollectorArmData* m_collectorArm;
	CollectorRollersData* m_collectorRollers;
	LauncherLoaderData* m_loaderData;
	BallHolderData* m_ballHolder;
	BallPusherData* m_ballPusher;
	float m_rollerSpeed;
	float m_extendTime;
	Timer m_timer;
	bool m_timerStarted;
};

#endif
