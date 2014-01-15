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

double RobotState::GameTime()
{
	return m_gameTimer.Get();
}

double RobotState::TotalTime()
{
	return m_totalTimer.Get();
}

void RobotState::Update()
{
	m_instance->m_lastGameMode = m_instance->m_gameMode;
	if (RobotBase::getInstance().IsDisabled())
	{
		m_instance->m_gameMode = GameState::DISABLED;
		m_instance->m_gameTimer.Stop();
		m_instance->m_gameTimer.Reset();
	}
	else if (RobotBase::getInstance().IsAutonomous())
	{
		m_instance->m_gameMode = GameState::AUTON;
		m_instance->m_gameTimer.Start();
	}
	else if (RobotBase::getInstance().IsOperatorControl())
	{
		m_instance->m_gameMode = GameState::TELEOP;
		m_instance->m_gameTimer.Start();
	}
}
