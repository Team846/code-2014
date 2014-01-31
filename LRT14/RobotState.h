#ifndef ROBOT_STATE_H_
#define ROBOT_STATE_H_

#include <RobotBase.h>
#include <Timer.h>
#include <DriverStation.h>

#include "Rhesus/Toolkit/GameState.h"

using namespace Rhesus::Toolkit;

class RobotState
{
public:
	static void Initialize();
	static RobotState& Instance();
	static void Finalize();
	
	GameState::Enum GameMode();
	GameState::Enum LastGameMode();
	DriverStation::Alliance Alliance();
	bool FMSAttached();
	double MatchTime(); // Seconds
	double TotalTime(); // Seconds
	double LastCycleTime(); // Seconds
	
	void Update();
	
private:
	RobotState();
	~RobotState();
	
	static RobotState *m_instance;
	
	GameState::Enum m_gameMode;
	GameState::Enum m_lastGameMode;
	DriverStation::Alliance m_alliance;
	Timer m_matchTimer;
	Timer m_totalTimer;
	double m_lastTime;
	double m_currentTime;
	bool m_fms;
	
	DISALLOW_COPY_AND_ASSIGN(RobotState);
};

#endif
