#ifndef LRT_ROBOT_14_H_
#define LRT_ROBOT_14_H_

#include "Rhesus/Toolkit/Rhesus.h"

#include <wdLib.h>
#include <sysLib.h>

#include "LRTRobotBase.h"

#include <stdio.h>

class LRTRobot14 : public LRTRobotBase
{
	public:
		static LRTRobot14* Robot();
	
		LRTRobot14();
		~LRTRobot14();
		
		void RobotInit();
	
		void Main();
		
		Rhesus::Toolkit::RobotState::Enum GameState();
		Rhesus::Toolkit::RobotState::Enum LastGameState();
		
	private:
		static LRTRobot14* mInstance;
		
		void UpdateGameState();
		
		Timer timer;
		
		WDOG_ID _watchdog;
		
		Rhesus::Toolkit::RobotState::Enum mGameState;
		Rhesus::Toolkit::RobotState::Enum mLastGameState;
};

#endif
