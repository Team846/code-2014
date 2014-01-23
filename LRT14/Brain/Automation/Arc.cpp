#include "Arc.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Utils/Util.h"
#include "../../Sensors/DriveEncoders.h"
#include "../../Config/RobotConfig.h"

Arc::Arc(double distance, double angle, double maxSpeed, double errorThreshold, bool continuous) :
	Automation("Arc"),
	Configurable("ArcTurn")
{
	m_distance = distance;
	m_angle = angle;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_continuous = continuous;
	m_arcGain = 1.0;
}

void Arc::AllocateResources()
{
	AllocateResource(ControlResource::DRIVE);
	AllocateResource(ControlResource::TURN);
}

bool Arc::Start()
{
	// Arc length = angle * distance * PI / (180 * sin(angle))
	// Resulting angle = 2 * angle
	// Central angle = 2 * angle
	// Radius = Chord Length / (2 * sin(Angle))
	m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	m_driveDistance = m_angle * m_distance * acos(-1) / (180 * (sin(m_angle * acos(-1) / 180)));
	m_turnAngle = 2 * m_angle;
	m_startDistance = m_drivetrain->GetPositionSetpoint(DrivetrainData::FORWARD);
	m_startAngle = DriveEncoders::Get()->GetTurnAngle();
	m_drive = new Drive(m_driveDistance, m_maxSpeed, m_errorThreshold, m_continuous);
	return m_drive->StartAutomation(GetStartEvent());
}

bool Arc::Run()
{
	bool driveComplete = m_drive->Update();
	
	double turnForwardCompletionDifference = (DriveEncoders::Get()->GetTurnAngle() - m_startAngle) / m_turnAngle
			- (DriveEncoders::Get()->GetRobotDist() - m_startDistance) / m_driveDistance; // Turn vs. forward path completion proportion difference
	
	double radius = fabs(m_driveDistance / (m_turnAngle * acos(-1) / 180.0)); // Radius = Arc Length (distance setpoint) / Central Angle (angle setpoint to radians)
	double velocitySetpoint = m_drivetrain->GetVelocitySetpoint(DrivetrainData::FORWARD)
			* RobotConfig::ROBOT_WIDTH / (2 * radius); // Match turn rate to forward rate
	velocitySetpoint *= Util::Sign(m_turnAngle) * Util::Sign(m_driveDistance);
	
	velocitySetpoint += m_arcGain * -turnForwardCompletionDifference * Util::Sign(m_turnAngle); // Correct for turn vs. forward proportion difference
	if (driveComplete && fabs(DriveEncoders::Get()->GetTurnAngle() - (m_startDistance + m_driveDistance)) < m_errorThreshold)
	{
		m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::POSITION_CONTROL);
		m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::TURN, m_turnAngle);
		return true;
	}
	return false;
}

bool Arc::Abort()
{
	if (m_drive->AbortAutomation(GetAbortEvent()))
	{
		m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::POSITION_CONTROL);
		m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::TURN, m_turnAngle);
		delete m_drive;
		return true;
	}
	return false;
}

void Arc::Configure()
{
	m_arcGain = GetConfig("ArcGain", 1.0);
}
