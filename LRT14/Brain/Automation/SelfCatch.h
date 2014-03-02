#ifndef SELF_CATCH_H_
#define SELF_CATCH_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/LauncherAngleData.h"
#include "../../Config/Configurable.h"
#include "Drive.h"
#include "Turn.h"
#include "Fire.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Fires the ball with the short shot and catches it.
 */
class SelfCatch : public Automation, public Configurable
{
public:
	SelfCatch();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	CollectorArmData* m_collectorArm;
	LauncherAngleData* m_launcherAngle;
	Drive* m_drive;
	Turn* m_turn;
	Fire* m_fire;
	AnalogChannel* m_ultrasonic;
	bool m_fired;
	int m_ballEnterDistance;
	
	float m_launchVelocity;
	float m_launchAngle;
};

#endif
