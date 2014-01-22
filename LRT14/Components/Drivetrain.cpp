#include "Drivetrain.h"

#include "../Config/ConfigRuntime.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/RobotConfig.h"
#include "../Sensors/DriveEncoders.h"
#include "../Utils/Util.h"
#include "../Actuators/DriveESC.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/AsyncCANJaguar.h"
#include "../Actuators/LRTJaguar.h"

Drivetrain::Drivetrain() :
	Component("Drivetrain", DriverStationConfig::DigitalIns::DRIVETRAIN, true),
	Configurable("Drivetrain")
{
	m_driveEncoders = new DriveEncoders(ConfigPortMappings::Get("Digital/LEFT_DRIVE_ENCODER_A"),
			ConfigPortMappings::Get("Digital/LEFT_DRIVE_ENCODER_B"),
			ConfigPortMappings::Get("Digital/RIGHT_DRIVE_ENCODER_A"),
			ConfigPortMappings::Get("Digital/RIGHT_DRIVE_ENCODER_B"));
//	m_escs[LEFT] = new DriveESC(new AsyncCANJaguar(ConfigPortMappings::Get("CAN/LEFT_DRIVE_A"), "LeftDriveA"),
//			new AsyncCANJaguar(ConfigPortMappings::Get("CAN/LEFT_DRIVE_B"), "LeftDriveB"),
//			m_driveEncoders->GetEncoder(DriveEncoders::LEFT), "LeftDriveESC");
//	m_escs[RIGHT] = new DriveESC(new AsyncCANJaguar(ConfigPortMappings::Get("CAN/RIGHT_DRIVE_A"), "RightDriveA"),
//			new AsyncCANJaguar(ConfigPortMappings::Get("CAN/RIGHT_DRIVE_B"), "RightDriveB"),
//			m_driveEncoders->GetEncoder(DriveEncoders::RIGHT), "RightDriveESC");
	m_escs[LEFT] = new DriveESC(new LRTJaguar(ConfigPortMappings::Get("PWM/LEFT_DRIVE_A"), "LeftDriveA", ConfigPortMappings::Get("Digital/LEFT_BRAKE_A")),
			new LRTJaguar(ConfigPortMappings::Get("PWM/LEFT_DRIVE_B"), "LeftDriveB", ConfigPortMappings::Get("Digital/LEFT_BRAKE_B")),
			m_driveEncoders->GetEncoder(DriveEncoders::LEFT), "LeftDriveESC");
	m_escs[RIGHT] = new DriveESC(new LRTJaguar(ConfigPortMappings::Get("PWM/RIGHT_DRIVE_A"), "RightDriveA", ConfigPortMappings::Get("Digital/RIGHT_BRAKE_A")),
			new LRTJaguar(ConfigPortMappings::Get("PWM/RIGHT_DRIVE_B"), "RightDriveB", ConfigPortMappings::Get("Digital/RIGHT_BRAKE_B")),
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
//		if (!m_drivetrainData->SyncingArc() || axis == DrivetrainData::FORWARD)
//		{
			m_PIDs[POSITION][axis].SetInput(axis == DrivetrainData::FORWARD ? m_driveEncoders->GetRobotDist() : m_driveEncoders->GetTurnAngle());
			m_PIDs[POSITION][axis].SetSetpoint(positionSetpoint);
			velocitySetpoint += m_PIDs[POSITION][axis].Update(1.0 / RobotConfig::LOOP_RATE);
			if (fabs(velocitySetpoint)> m_drivetrainData->GetPositionControlMaxSpeed(axis))
				velocitySetpoint = Util::Sign(velocitySetpoint) * m_drivetrainData->GetPositionControlMaxSpeed(axis);
//		}
//		else // Turn control in arc syncing mode
//		{
//			m_PIDs[POSITION][axis].setInput(m_drivetrainData->getRelativePositionSetpoint(FORWARD)
//					/ (m_drivetrainData->getAbsolutePositionSetpoint(FORWARD)
//							- m_drivetrainData->getPositionControlStartingPosition(FORWARD))
//					- m_drivetrainData->getRelativePositionSetpoint(TURN)
//					/ (m_drivetrainData->getAbsolutePositionSetpoint(TURN)
//							- m_drivetrainData->getPositionControlStartingPosition(TURN))); // Turn vs. forward proportion difference
//			m_PIDs[POSITION][axis].setSetpoint(0.0);
//			
//			/* 
//			 * To get a turn velocity from a forward velocity and a desired arc radius, use the following formula:
//			 * Turn = Forward * Robot Width / (2 * Desired Radius),
//			 * derived from arc radius formula (Desired Radius = Robot Width / (1 - Slower Wheel / Faster Wheel) - Robot Width / 2,
//			 * where Slower Wheel = Forward - Turn and Faster Wheel = Forward + Turn).
//			 * Turn is always the same sign as Forward, so add a sign to velocitySetpoint.
//			 * -RC 10/16/2013
//			 */
//			double radius = fabs((m_drivetrainData->getAbsolutePositionSetpoint(FORWARD)
//					- m_drivetrainData->getPositionControlStartingPosition(FORWARD))
//					/ ((m_drivetrainData->getAbsolutePositionSetpoint(TURN)
//					- m_drivetrainData->getPositionControlStartingPosition(TURN)) * acos(-1) / 180.0)); // Radius = Arc Length (distance setpoint) / Central Angle (angle setpoint to radians)
//			velocitySetpoint = m_drivetrainData->getVelocitySetpoint(FORWARD)
//					* RobotConfig::ROBOT_WIDTH / (2 * radius); // Match turn rate to forward rate
//			velocitySetpoint *= Util::Sign(m_drivetrainData->getAbsolutePositionSetpoint(TURN)
//					- m_drivetrainData->getPositionControlStartingPosition(TURN))
//					* Util::Sign(m_drivetrainData->getAbsolutePositionSetpoint(FORWARD)
//					- m_drivetrainData->getPositionControlStartingPosition(FORWARD));
//			AsyncPrinter::Printf("Turn velocity: %f %f %f %f %f %f\n", velocitySetpoint, m_drivetrainData->getAbsolutePositionSetpoint(FORWARD)
//					- m_drivetrainData->getPositionControlStartingPosition(FORWARD), m_drivetrainData->getAbsolutePositionSetpoint(TURN)
//					- m_drivetrainData->getPositionControlStartingPosition(TURN), m_driveEncoders->getWheelDist(LEFT), m_driveEncoders->getWheelDist(RIGHT), radius);
//			
//			velocitySetpoint += m_arcGain / m_PIDs[POSITION][axis].getProportionalGain() * Util::Sign(m_drivetrainData->getAbsolutePositionSetpoint(TURN)
//					- m_drivetrainData->getPositionControlStartingPosition(TURN)) * m_PIDs[POSITION][axis].update(
//					1.0 / RobotConfig::LOOP_RATE); // Correction for turn vs. forward proportion difference
//			AsyncPrinter::Printf("Turn velocitySetpoint: %f\n", velocitySetpoint);
//		}
//		m_drivetrainData->SetVelocitySetpoint(axis, velocitySetpoint);
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

	leftOutput = Util::Clamp<double>(leftOutput, -1.0, 1.0);
	rightOutput = Util::Clamp<double>(rightOutput, -1.0, 1.0);

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
}

void Drivetrain::UpdateDisabled()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
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
	SendToNetwork(m_driveEncoders->GetTurnTicks(), "TurnTicks", "RobotData");
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
