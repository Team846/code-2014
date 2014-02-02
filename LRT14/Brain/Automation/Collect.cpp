#include "Collect.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../RobotState.h"

Collect::Collect() :
	Automation("Collect", true),
	Configurable("Collect")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_gearTooth = SensorFactory::GetGearTooth(ConfigPortMappings::Get("Digital/COLLECTOR_GEAR_TOOTH"));
	m_redChannel = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/COLOR_RED"));
	m_greenChannel = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/COLOR_GREEN"));
	m_blueChannel = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/COLOR_BLUE"));
}

void Collect::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
}

bool Collect::Start()
{
	return true;
}

bool Collect::Run()
{
	if (!Aborting())
	{
		m_collectorArm->SetPosition(CollectorArmData::COLLECT);
		m_collectorRollers->SetRunning(true);
		m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
		m_collectorRollers->SetSpeed(1.0);
		bool wrongBall = false;
		if (RobotState::Instance().Alliance() == DriverStation::kRed)
		{
			if (m_redChannel->GetAverageValue() <= m_blueBallRed + m_colorRange
					&& m_redChannel->GetAverageValue() >= m_blueBallRed - m_colorRange
					&& m_blueChannel->GetAverageValue() <= m_blueBallBlue + m_colorRange
					&& m_blueChannel->GetAverageValue() >= m_blueBallBlue - m_colorRange
					&& m_greenChannel->GetAverageValue() <= m_blueBallGreen + m_colorRange
					&& m_greenChannel->GetAverageValue() >= m_blueBallGreen - m_colorRange)
			{
				m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
				wrongBall = true;
			}
		}
		if (RobotState::Instance().Alliance() == DriverStation::kBlue)
		{
			if (m_redChannel->GetAverageValue() <= m_redBallRed + m_colorRange
					&& m_redChannel->GetAverageValue() >= m_redBallRed - m_colorRange
					&& m_blueChannel->GetAverageValue() <= m_redBallBlue + m_colorRange
					&& m_blueChannel->GetAverageValue() >= m_redBallBlue - m_colorRange
					&& m_greenChannel->GetAverageValue() <= m_redBallGreen + m_colorRange
					&& m_greenChannel->GetAverageValue() >= m_redBallGreen - m_colorRange)
			{
				m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
				wrongBall = true;
			}
		}
		
		if (1 / m_gearTooth->GetPeriod() < m_ballCollectionThreshold && !wrongBall)
		{
			m_collectorRollers->SetRunning(false);
			return true;
		}
		return false;
	}
	else
	{
		m_collectorArm->SetPosition(CollectorArmData::STOWED);
		m_collectorRollers->SetRunning(false);
		return true;
	}
}

bool Collect::Abort()
{
	return true;
}

void Collect::Configure()
{
	m_ballCollectionThreshold = GetConfig("ball_collection_threshold", 0);
	m_redBallRed = GetConfig("red_ball_red", 0);
	m_redBallBlue = GetConfig("red_ball_blue", 0);
	m_redBallGreen = GetConfig("red_ball_green", 0);
	m_blueBallRed = GetConfig("blue_ball_red", 0);
	m_blueBallBlue = GetConfig("blue_ball_blue", 0);
	m_blueBallGreen = GetConfig("blue_ball_green", 0);
	m_colorRange = GetConfig("color_range", 5);
}