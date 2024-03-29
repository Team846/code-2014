#ifndef PASS_H_
#define PASS_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../Config/Configurable.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Passes the ball by releasing from the bumper.
 */
class Pass : public Automation, public Configurable
{
public:
	Pass();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	CollectorArmData* m_collectorArm;
	CollectorRollersData* m_rollersData;
	GearTooth* m_gearTooth;
	DigitalInput* m_proximity;
	float m_ballReleaseDistance;
	int m_startTicks;
	bool m_ballPassed;
	bool m_passingToGround;
	float m_restSpeed;
	float m_rollerSpeed;
	float m_gain;
	float m_rollerMaxSpeed;
	float m_rollerMaxSurfaceSpeed;
	float m_ticksToSurface;
	float m_threshold;
};

#endif
