#ifndef LRT_ROBOT_14_H_
#define LRT_ROBOT_14_H_

#include <WPILib.h>

#include <wdLib.h>
#include <sysLib.h>

#include "LRTRobotBase.h"

#include <stdio.h>

class LRTRobot14 : public LRTRobotBase
{
	public:
		LRTRobot14();
		~LRTRobot14();
		
		void RobotInit();
	
		void Tick();
		
		static bool maintenance;
		
	private:
		Timer timer;
		
		WDOG_ID _watchdog;
};

#endif
