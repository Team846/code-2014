#include "ActionManager.h"

#include "IRobotAction.h"
#include "../Defines.h"

using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Toolkit::Action;

BinarySemaphore ActionManager::s_sem(0);
std::queue<IRobotAction*> ActionManager::s_actionQueue;
Mutex ActionManager::s_so;
RhesusTask ActionManager::s_task = RhesusTask::CreateNew("ActionManager", (FUNCPTR) RunAll);

void ActionManager::Start()
{
	s_task.Start();
}

void ActionManager::RunAll()
{
	while(true)
	{
		
		s_sem.Take(WAIT_FOREVER);
		
		{

			lock_on l(s_so);
			
			while(!s_actionQueue.empty())
			{

				IRobotAction* act = s_actionQueue.front();
				s_actionQueue.pop();

				act->Run();

				DELETE(act); 
			}

		}

	}
}

void ActionManager::Add(IRobotAction* act)
{
	{
		lock_on l(s_so);
		
		s_actionQueue.push(act);
		
	}
	
	s_sem.Give();
}

void ActionManager::Clear()
{
	{
		lock_on l(s_so);
		
		while(!s_actionQueue.empty())
		{
			s_actionQueue.pop();
		}
		
	}
}
bool ActionManager::isDone()
{
	{
		lock_on l(s_so);
		return s_actionQueue.empty();
	}
}

