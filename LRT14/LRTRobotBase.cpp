#include "LRTRobotBase.h"
#include <sysLib.h>
#include <Rhesus/Toolkit/Tasks/TaskPool.h>

using namespace Rhesus::Toolkit::Tasks;

LRTRobotBase::LRTRobotBase()
: m_loopSynchronizer((TimerEventHandler) LRTRobotBase::ReleaseLoop, this)
{
	m_loopSynchSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	semTake(m_loopSynchSem, WAIT_FOREVER);
}

LRTRobotBase::~LRTRobotBase()
{
	printf("LRTRobotBase Destructing\n");
}

void LRTRobotBase::StartCompetition()
{
	printf("vxWorks task: %s\n", m_task->GetName()); //Maybe this line was crashing it...

	GetWatchdog().SetEnabled(false);
	m_loopSynchronizer.StartPeriodic(1.0 / 50.0);

	// Start the task pool
	TaskPool::Start(10);
	
	RobotInit();
	Main();

	// Stop the task pool
	TaskPool::Stop();
}

void LRTRobotBase::Main()
{
	while(true)
	{
		semTake(m_loopSynchSem, WAIT_FOREVER);
		Tick();
	}
}

void LRTRobotBase::Tick()
{
	printf("LRTRobotBase::Run(): Override me!");
}

void LRTRobotBase::ReleaseLoop(void *param)
{
	LRTRobotBase *robot = (LRTRobotBase *) param;
	semGive(robot->m_loopSynchSem);
}
