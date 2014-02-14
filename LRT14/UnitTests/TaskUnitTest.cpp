#ifdef UNIT_TEST

#include <WPILib.h>
#include <sysLib.h>

#include "RUnit/TestApparatus.h"
#include "../Rhesus/Toolkit/Tasks/TaskPool.h"
#include "../Rhesus/Toolkit/Tasks/BinarySemaphore.h"
#include "../Rhesus/Toolkit/IO/BufferedConsole.h"
#include "../Rhesus/Toolkit/Tasks/Mutex.h"
#include "../Rhesus/Toolkit/Tasks/lock_on.h"

using namespace RUnit;

using namespace Rhesus;
using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Toolkit::IO;

void TestPrint()
{
	printf("It Worked!\n");
}

void TestPrintWithNum(int d)
{
	printf("Print Task %d\n", d);
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
	
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 1);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 2);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 3);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 4);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 5);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 6);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 7);
	taskDelay(sysClkRateGet());
	TaskPool::EnqueueTask((FUNCPTR)TestPrintWithNum, 8);
	
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
	BinarySemaphore bin(0);
	TaskPool::Start();
	TaskPool::EnqueueTask((FUNCPTR)printThenGive, (UINT32)&bin);
	bin.Take();
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
	CountingSemaphore c(0);
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

INT32 TestBufferedConsole()
{
	std::printf("1: Starting TaskPool\n");
	TaskPool::Start();
	BufferedConsole::Printfln("SWAG");
	std::printf("2: Enqueing Print job\n");
	BufferedConsole::Printf("More Swag\n");
	BufferedConsole::Printfln("THWA%c", 'G');
	taskDelay(sysClkRateGet() * 10);
	std::printf("3: Stopping TaskPool\n");
	TaskPool::Stop();
	return 0;
}

void printThenUnlock(Mutex* m)
{
	{
		lock_on l(*m);
		std::printf("Unlocking...\n");
	}
}

INT32 TestMutex()
{
	TaskPool::Start();
	Mutex* m = new Mutex();
	
	TaskPool::EnqueueTask((FUNCPTR)printThenUnlock, (UINT32)m);
	
	taskDelay(sysClkRateGet());

	{
		lock_on l(*m);
		std::printf("Unlocked...\n");
	}
	
	TaskPool::Stop();
	
	return 0;
}

INT32 TestSemCIsEmpty()
{
	CountingSemaphore csem(3);
	
	RU_ASSERT(!csem.IsEmpty());
	
	return 0;
}

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
		app.RegisterTest((RU_FUNCPTR)TestBufferedConsole, "Test Buffered Console");
		
		//Mutex & lock_on: Working
		//app.RegisterTest((RU_FUNCPTR)TestMutex, "Test Mutex");
		
		//Semaphore Empty : Working
		//app.RegisterTest((RU_FUNCPTR)TestSemCIsEmpty, "Test Semaphore Empty");
		
		app.Run();
		
		return 0;
	}
}

#endif
