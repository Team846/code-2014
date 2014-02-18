#include "Collect.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../Config/DriverStationConfig.h"
#include "../../RobotState.h"
#include "../Events/JoystickReleasedEvent.h"
#include "../../DriverStation/LRTDriverStation.h"
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

Collect::Collect() :
	Automation("Collect", true),
	Configurable("Collect")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_redChannel = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/COLOR_RED"));
	m_greenChannel = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/COLOR_GREEN"));
	m_blueChannel = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/COLOR_BLUE"));
	m_hasBall = false;
}

void Collect::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
}

bool Collect::Start()
{
	m_hasBall = false;
	return true;
}

bool Collect::Run()
{
	if (!Aborting())
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
		m_collectorRollers->SetRunning(true);
		m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
		m_collectorRollers->SetSpeed(1.0);
		bool wrongBall = false;
		if (RobotState::Instance().Alliance() == DriverStation::kRed)
		{
			if (abs((int)m_redChannel->GetAverageValue() - m_blueBallRed) <= m_colorRange &&
					abs((int)m_blueChannel->GetAverageValue() - m_blueBallBlue) <= m_colorRange &&
					abs((int)m_greenChannel->GetAverageValue()- m_blueBallGreen) <= m_colorRange)
			{
				m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
				wrongBall = true;
			}
		}
		if (RobotState::Instance().Alliance() == DriverStation::kBlue)
		{
			if (abs((int)m_redChannel->GetAverageValue() - m_redBallRed) <= m_colorRange &&
					abs((int)m_blueChannel->GetAverageValue() - m_redBallBlue) <= m_colorRange &&
					abs((int)m_greenChannel->GetAverageValue()- m_redBallGreen) <= m_colorRange)
			{
				m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
				wrongBall = true;
			}
		}

		if (m_proximity->Get() == 0)
		{
			m_collectorRollers->SetRunning(false);
			m_hasBall = true;
		}
		return false;
	}
	else
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
		m_collectorRollers->SetRunning(false);
		return true;
	}
}

bool Collect::Abort()
{
	if (m_hasBall && dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())
			&& dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())->GetButton() == DriverStationConfig::JoystickButtons::COLLECT
			&& dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())->GetJoystick() == LRTDriverStation::Instance()->GetDriverStick())
	{
		return false;
	}
	return true;
}

void Collect::Configure()
{
	m_redBallRed = GetConfig("red_ball_red", 0);
	m_redBallBlue = GetConfig("red_ball_blue", 0);
	m_redBallGreen = GetConfig("red_ball_green", 0);
	m_blueBallRed = GetConfig("blue_ball_red", 0);
	m_blueBallBlue = GetConfig("blue_ball_blue", 0);
	m_blueBallGreen = GetConfig("blue_ball_green", 0);
	m_colorRange = GetConfig("color_range", 5);
}
