#ifndef ARC_H_
#define ARC_H_

#include "Automation.h"
#include "../../ComponentData/DrivetrainData.h"
#include "../../Config/Configurable.h"
#include "Drive.h"

/*!
 * @brief Drives the robot in a calculated arc turn.
 * To get a turn velocity from a forward velocity and a desired arc radius, use the following formula:
 * Turn = Forward * Robot Width / (2 * Desired Radius),
 * derived from arc radius formula (Desired Radius = Robot Width / (1 - Slower Wheel / Faster Wheel) - Robot Width / 2,
 * where Slower Wheel = Forward - Turn and Faster Wheel = Forward + Turn).
 * Turn is always the same sign as Forward, so add a sign to velocitySetpoint.
 */
class Arc : public Automation, public Configurable
{
public:
	Arc(double distance, double angle, double maxSpeed = 1.0, double errorThreshold = 0.5, bool continuous = false);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	DrivetrainData *m_drivetrain;

	double m_distance;
	double m_angle;
	double m_maxSpeed;
	double m_errorThreshold;
	bool m_continuous;
	
	Drive *m_drive;
	double m_startDistance;
	double m_startAngle;
	double m_driveDistance;
	double m_turnAngle;
	double m_arcGain;
};

#endif
