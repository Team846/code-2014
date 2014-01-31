#ifdef UNIT_TEST

#include <WPILib.h>
#include <sysLib.h>

#include "RUnit/TestApparatus.h"
#include "../Rhesus/Toolkit/Tasks/TaskPool.h"
#include "../Rhesus/Toolkit/Tasks/BinarySemaphore.h"

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

void printThenGive(BinarySemaphore* b)
{
	std::printf("YEE!!!!\n");
	sleep(5);
	b->Give();
}

INT32 TestBinarySemaphore()
{
	BinarySemaphore b = BinarySemaphore(SEM_EMPTY);
	TaskPool::Start();
	TaskPool::EnqueueTask((FUNCPTR)printThenGive, (UINT32)&b);
	b.Take();
	std::printf("WORLD CHAMPS HERE WE COME!!!\n");
	TaskPool::Stop();
	return 0;
}

void printThenGiveCounting(CountingSemaphore* c, UINT32 time)
{
	sleep(time);
	std::printf("Giving after %d...", time);
	c->Give();
}

INT32 TestCountingSemaphore()
{
	CountingSemaphore c = CountingSemaphore(0);
	TaskPool::Start();
	TaskPool::EnqueueTask((FUNCPTR)printThenGiveCounting, (UINT32)&c, 5);
	TaskPool::EnqueueTask((FUNCPTR)printThenGiveCounting, (UINT32)&c, 10);
	c.Take();
	std::printf("Recieved 1");
	c.Take();
	std::printf("Recieved 2");
	TaskPool::Stop();
	return 0;
}

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{      
		TestApparatus app;
		
		app.EnableOption(TestApparatus::OPT_BASIC);
		
		//app.RegisterTest((RU_FUNCPTR)TestTaskPool, "Test Task Pool");
		//app.RegisterTest((RU_FUNCPTR)TestTaskPoolOverflow, "Test Overflow");
		app.RegisterTest((RU_FUNCPTR)TestBinarySemaphore, "Test Binary Semaphore");
		//app.RegisterTest((RU_FUNCPTR)TestCountingSemaphore, "Test Counting Semaphore");
		
		app.Run();
		
		return 0;
	}
}

#endif
