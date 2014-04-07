#ifndef LOAD_LAUNCHER_H_
#define LOAD_LAUNCHER_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/PressurePlateData.h"
#include "../../Config/Configurable.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Loads the ball from the bumper into the launcher.
 */
class LoadLauncher : public Automation, public Configurable
{
public:
	LoadLauncher();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	CollectorArmData* m_collectorArm;
	CollectorRollersData* m_collectorRollers;
	LauncherLoaderData* m_loaderData;
	PressurePlateData* m_pressurePlate;
	DigitalInput* m_launcherProximity;
//	AnalogChannel* m_launcherProximity;
	DigitalInput* m_bumperProximity;
	float m_loadSpeed;
	bool m_pastIntermediate;
	bool m_ballIn;
	Timer m_timer;
	float m_ballSettleTime;
	bool m_timerStarted;
};

#endif
