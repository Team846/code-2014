#ifdef UNIT_TEST

#include <WPILib.h>
#include <sysLib.h>

#include "RUnit/TestApparatus.h"

using namespace RUnit;

INT32 TestDummy()
{
	RU_ASSERT(true, "The universe is broken.");
	
	return 0;
}

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{      
		TestApparatus app;
		
		app.EnableOption(TestApparatus::OPT_BASIC);
		
		app.RegisterTest((RU_FUNCPTR)TestDummy, "Dummy Task");
		
		app.Run();
		
		return 0;
	}
}

#endif
