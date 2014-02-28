#ifndef UNIT_TEST

#include "ScrubMonkey.h"

/*!
 * Pre-defined macro is:
 * START_ROBOT_CLASS(class);
 * 
 * The reason for expanding this macro is to make it easier to perform unit tests.
 */
RobotBase* FRC_userClassFactory()
{
	return new ScrubMonkey();
}

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{
		RobotBase::startRobotTask((FUNCPTR) FRC_userClassFactory);
		return 0;
	}
}

#endif
