#ifndef UNLOAD_LAUNCHER_H_
#define UNLOAD_LAUNCHER_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/PressurePlateData.h"
#include "../../Config/Configurable.h"
#include "../../Sensors/SensorFactory.h"
#include <Timer.h>

/*!
 * @brief Moves into human loading position.
 */
class HumanLoad : public Automation, public Configurable
{
public:
	HumanLoad();

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
	DigitalInput* m_proximity;
	bool m_hasBall;
	float m_rollerSpeed;
};

#endif
