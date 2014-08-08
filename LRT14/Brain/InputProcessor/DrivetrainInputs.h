#ifndef DRIVETRAIN_INPUTS_H_
#define DRIVETRAIN_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/DrivetrainData.h"
#include "../../Config/Configurable.h"

/*!
 * @brief Processes joystick information for controlling the drivetrain.
 */
class DrivetrainInputs : public InputProcessor, public Configurable
{
public:
	enum Axis
	{
		DRIVE,
		TURN
	};
	
	DrivetrainInputs(Axis axis);
	
	void Update();
	
	void Configure();
	
private:
	DebouncedJoystick* m_driver_stick;
	DebouncedJoystick* m_driver_wheel;
	
	DrivetrainData* drivetrainData;

	bool lastStop;
	double negInertiaScalar;
	double negInertiaAccumulator;
	double lastTurn;
	double stoppedForward;
	double stoppedTurn;
	int driveSign;
	int blendExponent;
	int turnExponent;
	int throttleExponent;
	double deadband;
	
	Axis m_axis;
};

#endif
