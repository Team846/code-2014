#ifndef PASS_H_
#define PASS_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/DrivetrainData.h"
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
	CollectorRollersData* m_loaderData;
	DrivetrainData* m_drivetrain;
	GearTooth* m_gearTooth;
	float m_ballReleaseDistance;
	int m_startTicks;
	float m_driveBackSpeed;
};

#endif
