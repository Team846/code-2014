#ifndef DRIVE_H_
#define DRIVE_H_

#include "Automation.h"
#include "../../ComponentData/DrivetrainData.h"
#include "../../Control/TrapezoidProfile.h"
#include "../../Config/Configurable.h"

/*!
 * @brief Drives forward or backward a set distance.
 */
class Drive : public Automation
{
public:
	Drive(double distance, double maxSpeed = 1.0, double errorThreshold = 0.5, bool continuous = false);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	DrivetrainData* m_drivetrain;
	
	double m_distance;
	double m_maxSpeed;
	double m_errorThreshold;
	bool m_continuous;
};

#endif
