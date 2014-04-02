#ifdef UNIT_TEST

#define TEST_SCRIPTS

#ifdef TEST_SCRIPTS

#include <WPILib.h>
#include <sysLib.h>

#include "RUnit/TestApparatus.h"
#include "../Rhesus/Toolkit/Tasks/TaskPool.h"
#include "../Rhesus/Toolkit/Tasks/ManagedTask.h"
#include "../Rhesus/Toolkit/Tasks/BinarySemaphore.h"
#include "../Rhesus/Toolkit/IO/BufferedConsole.h"
#include "../Rhesus/Toolkit/Tasks/Mutex.h"
#include "../Rhesus/Toolkit/Tasks/lock_on.h"
#include "../Rhesus/Toolkit/Tasks/do_async.h"
#include "../Rhesus/Toolkit/Tasks/do_async_static.h"

#include <Rhesus.Toolkit.Scripting.h>

using namespace RUnit;

using namespace Rhesus;
using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Toolkit::IO;

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{      
		std::printf("Starting tests...\n\n");
		
		TestApparatus app;
		
		app.EnableOption(TestApparatus::OPT_BASIC);
		
		//TaskPool: Working
		//app.RegisterTest((RU_FUNCPTR)TestTaskPool, "Test Task Pool");
		
		//TaskPool Overflow: Working
		//app.RegisterTest((RU_FUNCPTR)TestTaskPoolOverflow, "Test Overflow");
		
		//BinarySemaphore: Working
		//app.RegisterTest((RU_FUNCPTR)TestBinarySemaphore, "Test Binary Semaphore");
		
		//Counting Semaphore: Working
		//app.RegisterTest((RU_FUNCPTR)TestCountingSemaphore, "Test Counting Semaphore");
		
		//BufferedConsole: WORKING
		//app.RegisterTest((RU_FUNCPTR)TestBufferedConsole, "Test Buffered Console");
		
		//Mutex & lock_on: Working
		//app.RegisterTest((RU_FUNCPTR)TestMutex, "Test Mutex");
		
		//Semaphore Empty : Workingv
		//app.RegisterTest((RU_FUNCPTR)TestSemCIsEmpty, "Test Semaphore Empty");
		
		//do_async : Working
		//app.RegisterTest((RU_FUNCPTR)TestDoAsync, "Test do_async");
		
		//do_async_static: 
		//app.RegisterTest((RU_FUNCPTR)TestDoAsyncStatic, "Test do_async_static");
		
		//app.Run();
		
		return 0;
	}
}

#endif

#endif
