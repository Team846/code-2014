#ifdef UNIT_TEST

#include <WPILib.h>
#include <sysLib.h>

#include "RUnit/TestApparatus.h"
#include "../Rhesus/Toolkit/Tasks/TaskPool.h"

using namespace RUnit;

using namespace Rhesus;
using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Tasks;

void TestPrint()
{
	printf("It Worked!\n");
}

void TestPrintForever(int d)
{
	while(true)
	{
		printf("Print Task %d\n", d);
	}
}

INT32 TestTaskPool()
{
	RU_ASSERT(true, "The universe is broken.");
	
	TaskPool::Start(3);
	printf("Starting Worked!\n");
	TaskPool::EnqueueTask((FUNCPTR)TestPrint);
	
	sleep(5);
	
	TaskPool::Stop();
	
	return 0;
}

INT32 TestTaskPoolOverflow()
{
	std::printf("Starting overflow test.\n");
	TaskPool::Start(3);
	
	TaskPool::EnqueueTask((FUNCPTR)TestPrintForever, 1);
	std::printf("1 Worked\n");
	TaskPool::EnqueueTask((FUNCPTR)TestPrintForever, 2);
	std::printf("2 Worked\n");
	TaskPool::EnqueueTask((FUNCPTR)TestPrintForever, 3);
	std::printf("3 Worked\n");
	TaskPool::EnqueueTask((FUNCPTR)TestPrint);
	
	sleep(5);
	
	TaskPool::Stop();
	
	std::printf("\nEverything worked!\n");
	
	return 0;
}

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{      
		TestApparatus app;
		
		app.EnableOption(TestApparatus::OPT_BASIC);
		
		app.RegisterTest((RU_FUNCPTR)TestTaskPool, "Test Task Pool");
		app.RegisterTest((RU_FUNCPTR)TestTaskPoolOverflow, "Test Overflow");
		
		app.Run();
		
		return 0;
	}
}

#endif
