#ifndef TURN_H_
#define TURN_H_

#include "Automation.h"
#include "../../ComponentData/DrivetrainData.h"

/*!
 * @brief Turns a set angle.
 */
class Turn : public Automation
{
public:
	Turn(double angle, double maxSpeed = 1.0, double errorThreshold = 0.5);

	virtual bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
protected:
	Turn();
	
	void setAngle(double angle);
	void setMaxSpeed(double speed);
	void setErrorThreshold(double errorThreshold);
	
private:
	DrivetrainData* m_drivetrain;
	
	double m_angle;
	double m_maxSpeed;
	double m_errorThreshold;
};

#endif
