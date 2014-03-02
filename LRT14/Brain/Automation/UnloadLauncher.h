#ifndef UNLOAD_LAUNCHER_H_
#define UNLOAD_LAUNCHER_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../Config/Configurable.h"
#include "../../Sensors/SensorFactory.h"
#include <Timer.h>

/*!
 * @brief Moves the ball from the launcher to the collector.
 */
class UnloadLauncher : public Automation, public Configurable
{
public:
	UnloadLauncher();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	CollectorArmData* m_collectorArm;
	LauncherLoaderData* m_loaderData;
	float m_waitTime;
	Timer m_timer;
};

#endif
