#ifndef FIRE_H_
#define FIRE_H_

#include "Automation.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/LauncherAngleData.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/PressurePlateData.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Fires the ball.
 */
class Fire : public Automation, public Configurable
{
public:
	Fire();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
	void Configure();

private:
	LauncherLoaderData* m_loaderData;
	CollectorArmData* m_collectorArmData;
	PressurePlateData* m_pressurePlate;
	DigitalInput* m_proximity;
	Timer m_timer;
	float m_timeout;
	bool m_hasBall;
};

#endif
