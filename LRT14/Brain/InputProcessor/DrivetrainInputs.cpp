#include "DrivetrainInputs.h"
#include "../../Config/RobotConfig.h"

DrivetrainInputs::DrivetrainInputs(Axis axis) :
	Configurable("DrivetrainInputs"),
	m_axis(axis)
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_driver_wheel = LRTDriverStation::Instance()->GetDriverWheel();
	
	drivetrainData = DrivetrainData::Get();
	
	if (m_axis == DRIVE)
		RegisterResource(ControlResource::DRIVE);
	else if (m_axis == TURN)
		RegisterResource(ControlResource::TURN);
	
	lastStop = false;
	driveSign = -1;
}

void DrivetrainInputs::Update()
{
	double forward = pow(
			driveSign * m_driver_stick->GetAxis(Joystick::kYAxis), throttleExponent);

	int signForward = forward > 0 ? 1 : -1;

	if (fabs(forward) < deadband)
		forward = 0.0;
	else
	{
		forward -= signForward * deadband;
		forward /= 1.0 - deadband;
	}

	if (m_axis == DRIVE)
	{
		drivetrainData->SetVelocitySetpoint(DrivetrainData::FORWARD, forward);
		drivetrainData->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
	}
	else if (m_axis == TURN)
	{
		double turn = 0.0;
		turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
	//	turn = -m_driver_stick->GetAxis(Joystick::kZAxis);
		
		int sign = turn > 0 ? 1 : -1;
		
		turn = sign * pow(turn , turnExponent);
	
	
		// Blending routine
		double absForward = fabs(forward); // To ensure correct arc when switching direction
	
		double blend = pow((1 - absForward), blendExponent); // Always between 0 and 1, raised to an exponent to adjust transition between in place and arc.
	
		const double turnInPlace = turn; // Normal turn
		const double constRadiusTurn = turn * absForward; // Arc turn
	
		double turnComposite = turnInPlace * (blend) + constRadiusTurn * (1 - blend); // Blended function
		
		drivetrainData->SetVelocitySetpoint(DrivetrainData::TURN, turnComposite);
		drivetrainData->SetControlMode(DrivetrainData::TURN, DrivetrainData::VELOCITY_CONTROL);
	}

	if (m_driver_wheel->IsButtonJustPressed(DriverStationConfig::JoystickButtons::REVERSE_DRIVE))
	{
		driveSign = -driveSign;
	}
}

void DrivetrainInputs::Configure()
{
	blendExponent = GetConfig("blend_exponent", 1);
	turnExponent = GetConfig("turn_exponent", 2);
	throttleExponent = GetConfig("throttle_exponent", 1);
	deadband = GetConfig("deadband", 0.01);
}
