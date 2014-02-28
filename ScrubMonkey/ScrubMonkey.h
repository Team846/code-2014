#ifndef SCRUBMONKEY_H_
#define SCRUBMONKEY_H_

#include <WPILib.h>

#include <wdLib.h>
#include <sysLib.h>

#include "LRTRobotBase.h"

#include <stdio.h>

class ScrubMonkey : public LRTRobotBase
{
	public:
		ScrubMonkey();
		~ScrubMonkey();
		
		void RobotInit();
	
		void Tick();
		
	private:
		Timer timer;
		
		WDOG_ID _watchdog;
};

#endif
