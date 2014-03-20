#include "RobotState.h"

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
	//m_alliance = DriverStation::GetInstance()->GetAlliance();
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
	m_lastGameMode = m_gameMode;
	if (RobotBase::getInstance().IsDisabled())
	{
		m_gameMode = GameState::DISABLED;
		m_matchTimer.Stop();
		m_matchTimer.Reset();
	}
	else if (RobotBase::getInstance().IsAutonomous())
	{
		m_gameMode = GameState::AUTONOMOUS;
		m_matchTimer.Start();
	}
	else if (RobotBase::getInstance().IsOperatorControl())
	{
		m_gameMode = GameState::TELEOPERATED;
		m_matchTimer.Start();
	}
	//m_alliance = DriverStation::GetInstance()->GetAlliance();
	m_fms = DriverStation::GetInstance()->IsFMSAttached();
	m_lastTime = m_currentTime;
	m_currentTime = Timer::GetFPGATimestamp();
}
