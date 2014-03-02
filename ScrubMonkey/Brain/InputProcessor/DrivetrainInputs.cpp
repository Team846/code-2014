#include "DrivetrainInputs.h"
#include "../../Config/RobotConfig.h"

DrivetrainInputs::DrivetrainInputs()
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_driver_wheel = LRTDriverStation::Instance()->GetDriverWheel();
	
	drivetrainData = DrivetrainData::Get();
	RegisterResource(ControlResource::DRIVE);
	RegisterResource(ControlResource::TURN);
	
	lastStop = false;
	driveSign = 1;
}

void DrivetrainInputs::Update()
{
	drivetrainData->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::OPEN_LOOP);
	drivetrainData->SetControlMode(DrivetrainData::TURN, DrivetrainData::OPEN_LOOP);
	
	double turn = 0.0;
	turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
	
	int sign = turn > 0 ? 1 : -1;
	
	turn = sign * pow(turn , 2);

	//			turn *= turn * sign;

	double forward = pow(
			driveSign * m_driver_stick->GetAxis(Joystick::kYAxis),
			RobotConfig::Drive::THROTTLE_EXPONENT);

	int signForward = forward > 0 ? 1 : -1;

	if (fabs(forward) < RobotConfig::Drive::DEADBAND)
		forward = 0.0;
	else
	{
		forward -= signForward * RobotConfig::Drive::DEADBAND;
		forward /= 1.0 - RobotConfig::Drive::DEADBAND;
	}
	

	// Blending routine
	double absForward = fabs(forward); // To ensure correct arc when switching direction

	double blend = pow((1 - absForward), RobotConfig::Drive::BLEND_EXPONENT); // Always between 0 and 1, raised to an exponent to adjust transition between in place and arc.

	const double turnInPlace = turn; // Normal turn
	const double constRadiusTurn = turn * absForward; // Arc turn

	double turnComposite = turnInPlace * (blend) + constRadiusTurn * (1 - blend); // Blended function

	if (m_driver_wheel->IsButtonJustPressed(DriverStationConfig::JoystickButtons::REVERSE_DRIVE))
	{
		driveSign = -driveSign;
	}

	drivetrainData->SetOpenLoopOutput(DrivetrainData::FORWARD, forward);
	drivetrainData->SetOpenLoopOutput(DrivetrainData::TURN, turnComposite);
	
	
}
