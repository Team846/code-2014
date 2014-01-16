#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

#include "../Config/Configurable.h"
#include "Component.h"
#include "../Control/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Communication/LiveNetworkSendable.h"

class DriveESC;

class DriveEncoders;

/*!
 * @brief Provides control over the drivetrain component.
 */
class Drivetrain : public Component, public Configurable, public LiveNetworkSendable
{
public:
	enum Side
	{
		LEFT = 0,
		RIGHT = 1
	};
	
	Drivetrain();
	~Drivetrain();

	void OnEnabled(); 
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	
	void Send();

private:
	double ComputeOutput(DrivetrainData::Axis);
	void ConfigurePIDObject(PID *pid, std::string objName, bool feedForward);
	void ConfigureForwardCurrentLimit();
	void ConfigureReverseCurrentLimit();
	
	const static int POSITION = 0;
	const static int VELOCITY = 1;
	PID m_PIDs[2][2];
	DrivetrainData *m_drivetrainData;
	DriveEncoders* m_driveEncoders;
	DriveESC *m_escs[4];
};	

#endif
