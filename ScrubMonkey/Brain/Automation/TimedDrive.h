// SRINJOY DO THIS

#ifndef TIMED_DRIVE_H_
#define TIMED_DRIVE_H_

#include "Automation.h"
#include "../../ComponentData/DrivetrainData.h"
#include "../../Control/TrapezoidProfile.h"
#include "../../Config/Configurable.h"

/*!
 * @brief Drives forward or backward a set distance.
 */
class TimedDrive : public Automation, public Configurable
{
public:
	TimedDrive(double timeSeconds, double maxSpeed = 1.0);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	DrivetrainData* m_drivetrain;
	
	double m_distance;
	double m_maxSpeed;
	double m_errorThreshold;
	bool m_continuous;
	
	double m_start;
	TrapezoidProfile* m_profile;
	double m_maxVelocity;
	double m_timeToMax;
};

#endif
