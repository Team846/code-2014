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
	lastTurn = 0.0;
	negInertiaAccumulator = 0.0;
	constRadius = false;
}

void DrivetrainInputs::Update()
{
	double forward = -pow(m_driver_stick->GetAxis(Joystick::kYAxis), throttleExponent);

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
		drivetrainData->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
		drivetrainData->SetVelocitySetpoint(DrivetrainData::FORWARD, forward);
	}
	else if (m_axis == TURN)
	{
		double turn = 0.0;
		turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
	//	turn = -m_driver_stick->GetAxis(Joystick::kZAxis);
		
		int sign = turn > 0 ? 1 : -1;
		
		if (constRadius)
		{
			if (constRadiusTurnExponent % 2 == 0)
				turn = sign * pow(turn, constRadiusTurnExponent);
			else
				turn = pow(turn, constRadiusTurnExponent);
		}
		else
		{
			if (turnExponent % 2 == 0)
				turn = sign * pow(turn, turnExponent);
			else
				turn = pow(turn, turnExponent);
		}

		// Negative Inertia routine
		double negInertia = turn - lastTurn;
		lastTurn = turn;
		
		double negInertiaPower = negInertia * negInertiaScalar;
		negInertiaAccumulator += negInertiaPower;
		turn += negInertiaAccumulator;
		
		if (negInertiaAccumulator > 1) {
			negInertiaAccumulator -= 1;
		} else if (negInertiaAccumulator < -1) {
			negInertiaAccumulator += 1;
		} else {
			negInertiaAccumulator = 0;
		}
		
		// Blending routine
		double absForward = fabs(forward); // To ensure correct arc when switching direction
	
		double blend = pow((1 - absForward), blendExponent); // Always between 0 and 1, raised to an exponent to adjust transition between in place and arc.
	
		const double turnInPlace = turn; // Normal turn
		const double constRadiusTurn = turn * absForward; // Arc turn
	
		double turnComposite;
		
		if (constRadius && !m_driver_wheel->IsButtonDown(DriverStationConfig::JoystickButtons::QUICK_TURN))
			turnComposite = constRadiusTurn;
		else
			turnComposite = turnInPlace * (blend) + constRadiusTurn * (1 - blend); // Blended function

		drivetrainData->SetControlMode(DrivetrainData::TURN, DrivetrainData::VELOCITY_CONTROL);
		drivetrainData->SetVelocitySetpoint(DrivetrainData::TURN, turnComposite);
	}

	if (m_driver_wheel->IsButtonJustPressed(DriverStationConfig::JoystickButtons::REVERSE_DRIVE))
	{
		constRadius = !constRadius;
	}
}

void DrivetrainInputs::Configure()
{
	blendExponent = GetConfig("blend_exponent", 1);
	turnExponent = GetConfig("turn_exponent", 2);
	constRadiusTurnExponent = GetConfig("const_radius_turn_exponent", 1);
	throttleExponent = GetConfig("throttle_exponent", 1);
	deadband = GetConfig("deadband", 0.01);
	negInertiaScalar = GetConfig("neg_inertia_scalar", 5.0);
}
