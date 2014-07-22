#include "DrivetrainInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

DrivetrainInputs::DrivetrainInputs() :
	Configurable("DrivetrainInputs")
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_driver_wheel = LRTDriverStation::Instance()->GetDriverWheel();
	
	drivetrainData = DrivetrainData::Get();
	
	//Dashboard2::AddTelemetryData("Collector Arm Input", (INT16)DashboardTelemetryID::COLLECTOR_ARM_INPUT, DashboardTelemetryType::STRING);
	
	RegisterResource(ControlResource::DRIVE);
	RegisterResource(ControlResource::TURN);
	
	lastStop = false;
	driveSign = -1;
}

void DrivetrainInputs::Update()
{
	drivetrainData->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
	drivetrainData->SetControlMode(DrivetrainData::TURN, DrivetrainData::VELOCITY_CONTROL);
	
	double turn = 0.0;
	turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
//	turn = -m_driver_stick->GetAxis(Joystick::kZAxis);
	
	int sign = turn > 0 ? 1 : -1;
	
	turn = sign * pow(turn , turnExponent);

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

	// Blending routine
	double absForward = fabs(forward); // To ensure correct arc when switching direction

	double blend = pow((1 - absForward), blendExponent); // Always between 0 and 1, raised to an exponent to adjust transition between in place and arc.

	const double turnInPlace = turn; // Normal turn
	const double constRadiusTurn = turn * absForward; // Arc turn

	double turnComposite = turnInPlace * (blend) + constRadiusTurn * (1 - blend); // Blended function

	if (m_driver_wheel->IsButtonJustPressed(DriverStationConfig::JoystickButtons::REVERSE_DRIVE))
	{
		driveSign = -driveSign;
	}

	drivetrainData->SetVelocitySetpoint(DrivetrainData::FORWARD, forward);
	drivetrainData->SetVelocitySetpoint(DrivetrainData::TURN, turnComposite);
}

void DrivetrainInputs::Configure()
{
	blendExponent = GetConfig("blend_exponent", 1);
	turnExponent = GetConfig("turn_exponent", 2);
	throttleExponent = GetConfig("throttle_exponent", 1);
	deadband = GetConfig("deadband", 0.01);
}
