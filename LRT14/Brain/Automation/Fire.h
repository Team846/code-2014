#ifndef FIRE_H_
#define FIRE_H_

#include "Automation.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/LauncherAngleData.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Fires the ball.
 */
class Fire : public Automation
{
public:
	Fire();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	LauncherLoaderData* m_loaderData;
	CollectorArmData* m_collectorArmData;
	DigitalInput* m_proximity;
};

#endif
