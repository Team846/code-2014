#include "RobotState.h"
#include "Communication/Dashboard2.h"
#include "Communication/DashboardTelemetryID.h"

RobotState *RobotState::m_instance = NULL;

void RobotState::Initialize()
{
	if (m_instance == NULL)
		m_instance = new RobotState();
}

RobotState& RobotState::Instance()
{
	if (m_instance == NULL)
		m_instance = new RobotState();
	return *m_instance;
}

void RobotState::Finalize()
{
	delete m_instance;
}

RobotState::RobotState()
{
	m_totalTimer.Start();
	m_gameMode = GameState::DISABLED;
	m_lastTime = 0.0;
	m_fms = DriverStation::GetInstance()->IsFMSAttached();
}

RobotState::~RobotState()
{
}

GameState::Enum RobotState::GameMode()
{
	return m_gameMode;
}

GameState::Enum RobotState::LastGameMode()
{
	return m_lastGameMode;
}

double RobotState::MatchTime()
{
	return m_matchTimer.Get();
}

double RobotState::TotalTime()
{
	return m_totalTimer.Get();
}

bool RobotState::FMSAttached()
{
	return m_fms;
}

double RobotState::LastCycleTime()
{
	return m_currentTime - m_lastTime;
}

void RobotState::Update()
{
	std::string gameModeStr = "???";
	
	m_lastGameMode = m_gameMode;
	if (RobotBase::getInstance().IsDisabled())
	{
		gameModeStr = "DISABLED";
		m_gameMode = GameState::DISABLED;
		m_matchTimer.Stop();
		m_matchTimer.Reset();
	}
	else if (RobotBase::getInstance().IsAutonomous())
	{
		gameModeStr = "AUTONOMOUS";
		m_gameMode = GameState::AUTONOMOUS;
		m_matchTimer.Start();
	}
	else if (RobotBase::getInstance().IsOperatorControl())
	{
		gameModeStr = "TELEOP";
		m_gameMode = GameState::TELEOPERATED;
		m_matchTimer.Start();
	}
	m_fms = DriverStation::GetInstance()->IsFMSAttached();
	m_lastTime = m_currentTime;
	m_currentTime = Timer::GetFPGATimestamp();
	
	Dashboard2::SetOrAddTelemetryData("Game State", (INT16)DashboardTelemetryID::GAME_STATE, DashboardTelemetryType::STRING, gameModeStr);
}
