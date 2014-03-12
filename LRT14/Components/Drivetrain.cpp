#include "Drivetrain.h"

#include <Rhesus.Toolkit.Utilities.h>

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/RobotConfig.h"
#include "../Sensors/DriveEncoders.h"
#include "../Actuators/DriveESC.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTJaguar.h"
#include "../Actuators/LRTTalon.h"

#include "../Communication/Dashboard2.h"

using namespace Rhesus::Toolkit::Utilities;

Drivetrain::Drivetrain() :
	Component("Drivetrain", DriverStationConfig::DigitalIns::DRIVETRAIN),
	Configurable("Drivetrain")
{
	m_driveEncoders = new DriveEncoders(ConfigPortMappings::Get("Digital/LEFT_DRIVE_ENCODER_A"),
			ConfigPortMappings::Get("Digital/LEFT_DRIVE_ENCODER_B"),
			ConfigPortMappings::Get("Digital/RIGHT_DRIVE_ENCODER_A"),
			ConfigPortMappings::Get("Digital/RIGHT_DRIVE_ENCODER_B"));
	m_leftA = new LRTTalon(ConfigPortMappings::Get("PWM/LEFT_DRIVE_A"), "LeftDriveA", ConfigPortMappings::Get("Digital/LEFT_BRAKE_A"));
	m_leftB = new LRTTalon(ConfigPortMappings::Get("PWM/LEFT_DRIVE_B"), "LeftDriveB", ConfigPortMappings::Get("Digital/LEFT_BRAKE_B"));
	m_rightA = new LRTTalon(ConfigPortMappings::Get("PWM/RIGHT_DRIVE_A"), "RightDriveA", ConfigPortMappings::Get("Digital/RIGHT_BRAKE_A"));
	m_rightB = new LRTTalon(ConfigPortMappings::Get("PWM/RIGHT_DRIVE_B"), "RightDriveB", ConfigPortMappings::Get("Digital/RIGHT_BRAKE_B"));
	m_escs[LEFT] = new DriveESC(m_leftA, m_leftB,
			m_driveEncoders->GetEncoder(DriveEncoders::LEFT), "LeftDriveESC");
	m_escs[RIGHT] = new DriveESC(m_rightA, m_rightB,
			m_driveEncoders->GetEncoder(DriveEncoders::RIGHT), "RightDriveESC");
	m_drivetrainData = DrivetrainData::Get();
	
	Kv = 1;
}

Drivetrain::~Drivetrain()
{

}

double Drivetrain::ComputeOutput(DrivetrainData::Axis axis)
{
	double positionSetpoint = m_drivetrainData->GetPositionSetpoint(axis);
	double velocitySetpoint = m_drivetrainData->GetVelocitySetpoint(axis);
	double rawOutput = m_drivetrainData->GetOpenLoopOutput(axis);

	switch (m_drivetrainData->GetControlMode(axis))
	{
	case DrivetrainData::POSITION_CONTROL:
		m_PIDs[POSITION][axis].SetInput(axis == DrivetrainData::FORWARD ? m_driveEncoders->GetRobotDist() : m_driveEncoders->GetTurnAngle());
		m_PIDs[POSITION][axis].SetSetpoint(positionSetpoint);
		velocitySetpoint += m_PIDs[POSITION][axis].Update(1.0 / RobotConfig::LOOP_RATE);
		if (fabs(velocitySetpoint) > m_drivetrainData->GetPositionControlMaxSpeed(axis))
			velocitySetpoint = MathUtils::Sign(velocitySetpoint) * m_drivetrainData->GetPositionControlMaxSpeed(axis);
	case DrivetrainData::VELOCITY_CONTROL:
		if (fabs(velocitySetpoint) < 2.0E-2)
			m_PIDs[VELOCITY][axis].SetIIREnabled(true);
		else
			m_PIDs[VELOCITY][axis].SetIIREnabled(false);

		if (axis == DrivetrainData::FORWARD)
			m_PIDs[VELOCITY][axis].SetInput(
					m_driveEncoders->GetNormalizedForwardSpeed());
		else
			m_PIDs[VELOCITY][axis].SetInput(
					m_driveEncoders->GetNormalizedTurningSpeed());

		m_PIDs[VELOCITY][axis].SetSetpoint(velocitySetpoint);

		rawOutput = m_PIDs[VELOCITY][axis].Update(1.0 / RobotConfig::LOOP_RATE);
		break;
	case DrivetrainData::OPEN_LOOP:
		break;
	}
	return rawOutput;
}

void Drivetrain::UpdateEnabled()
{
	double fwdOutput = ComputeOutput(DrivetrainData::FORWARD); //positive means forward
	double turnOutput = ComputeOutput(DrivetrainData::TURN); //positive means turning counter-clockwise. Matches the way driveencoders work.

	double leftOutput = fwdOutput - turnOutput;
	double rightOutput = fwdOutput + turnOutput;

	leftOutput = MathUtils::Clamp<double>(leftOutput, -1.0, 1.0);
	rightOutput = MathUtils::Clamp<double>(rightOutput, -1.0, 1.0);

	if (m_drivetrainData->ShouldOverrideForwardCurrentLimit())
	{
		m_escs[LEFT]->SetForwardCurrentLimit(m_drivetrainData->GetForwardCurrentLimit());
		m_escs[RIGHT]->SetForwardCurrentLimit(m_drivetrainData->GetForwardCurrentLimit());
	}
	else
	{
		ConfigureForwardCurrentLimit();
	}
	if (m_drivetrainData->ShouldOverrideReverseCurrentLimit())
	{
		m_escs[LEFT]->SetReverseCurrentLimit(m_drivetrainData->GetReverseCurrentLimit());
		m_escs[RIGHT]->SetReverseCurrentLimit(m_drivetrainData->GetReverseCurrentLimit());
	}
	else
	{
		ConfigureReverseCurrentLimit();
	}
	m_escs[LEFT]->SetDutyCycle(leftOutput);
	m_escs[RIGHT]->SetDutyCycle(rightOutput);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::DRIVETRAIN_LEFT_OUTPUT, (float)leftOutput);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::DRIVETRAIN_RIGHT_OUTPUT, (float)rightOutput);
}

void Drivetrain::UpdateDisabled()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
	m_leftA->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	m_leftB->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	m_rightA->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	m_rightB->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::DRIVETRAIN_LEFT_OUTPUT, 0.0f);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::DRIVETRAIN_RIGHT_OUTPUT, 0.0f);
}

void Drivetrain::OnEnabled()
{
}

void Drivetrain::OnDisabled()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
}

void Drivetrain::Configure()
{
	Kv = GetConfig("Kv", 1.0);
	
	ConfigurePIDObject(&m_PIDs[VELOCITY][DrivetrainData::TURN], "velocity_turn", true);
	ConfigurePIDObject(&m_PIDs[VELOCITY][DrivetrainData::FORWARD], "velocity_fwd", true);

	ConfigurePIDObject(&m_PIDs[POSITION][DrivetrainData::TURN], "position_turn", false);
	ConfigurePIDObject(&m_PIDs[POSITION][DrivetrainData::FORWARD], "position_fwd", false);

	ConfigureForwardCurrentLimit();
	ConfigureReverseCurrentLimit();
}

void Drivetrain::Send()
{
	Dashboard2::EnqueueDrivetrainTicksMessage(m_driveEncoders->GetTurnTicks());
	SendToNetwork(m_driveEncoders->GetRawTurningSpeed(), "TurnTicks", "RobotData");
	SendToNetwork(m_driveEncoders->GetRawForwardSpeed(), "Rate", "RobotData");
	SendToNetwork(m_driveEncoders->GetEncoder(DriveEncoders::LEFT)->GetRate(), "LeftTicks", "RobotData");
	SendToNetwork(m_driveEncoders->GetEncoder(DriveEncoders::RIGHT)->GetRate(), "RightTicks", "RobotData");
}

void Drivetrain::ConfigurePIDObject(PID *pid, std::string objName, bool feedForward)
{
	double p = GetConfig(objName + "_P", 2.0);
	double i = GetConfig(objName + "_I", 0.0);
	double d = GetConfig(objName + "_D", 0.0);

	pid->SetParameters(p, i, d, 1.0, 0.87, feedForward);
}

void Drivetrain::ConfigureForwardCurrentLimit()
{
	m_escs[LEFT]->SetForwardCurrentLimit(GetConfig("forwardCurrentLimit", 50.0 / 100.0));
	m_escs[RIGHT]->SetForwardCurrentLimit(GetConfig("forwardCurrentLimit", 50.0 / 100.0));
}

void Drivetrain::ConfigureReverseCurrentLimit()
{
	m_escs[LEFT]->SetReverseCurrentLimit(GetConfig("reverseCurrentLimit", 50.0 / 100.0));
	m_escs[RIGHT]->SetReverseCurrentLimit(GetConfig("reverseCurrentLimit", 50.0 / 100.0));
}
