#include "TaskPool.h"

#include <string>
#include <algorithm>

#include <errnoLib.h>

#include "lock_ptr.h"

#include "../Utilities/StringUtil.h"
#include "../Utilities/ContainerCleanup.h"

#include "../lexical_cast.h"

#include "../Defines.h"

using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Toolkit::Utilities;

std::queue<TaskPool::taskStructure> TaskPool::s_taskQ;
Mutex TaskPool::s_taskQSyncObj;
CountingSemaphore TaskPool::s_taskSignal(0);
std::vector<Task*> TaskPool::s_tasks;
Mutex TaskPool::s_tasksMutex;
std::vector<Task*> TaskPool::s_tempTasks;
CountingSemaphore* TaskPool::s_availableTasks = NULL;
bool TaskPool::s_isRunning(false);

bool TaskPool::IsRunning()
{
	return s_isRunning;
}

void TaskPool::Start()
{
	//Starts with 5 worker threads by default
	Start(5);
}

void TaskPool::Start(INT32 numThreads)
{
	s_availableTasks = new CountingSemaphore(numThreads);
	
	std::printf("Task available: %s\n", (s_availableTasks->IsEmpty() ? "false" : "true"));
	
	for(int i = 0; i < numThreads; i++)
	{
		std::string name = "TaskPool Worker Thread #" + lexical_cast<int>(i + 1);
		
		Task* task = new Task(name.c_str(), (FUNCPTR)WorkerTask, Task::kDefaultPriority - 1);
		task->Start();
		
		s_tasks.push_back(task);
	}
	
	s_isRunning = true;
}

void TaskPool::EnqueueTask(FUNCPTR ptr, UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4,
		UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8)
{
	taskStructure task;
	
	memcpy(&task, &ptr, sizeof(FUNCPTR) + 9 * sizeof(UINT32)); // one-liner!
	
	if(s_availableTasks->IsEmpty())
	{
		lock_ptr<std::vector<Task*> > tPtr(s_tasks, s_tasksMutex);
		{
			std::string name = "TaskPool Worker Thread #" + lexical_cast<int>(tPtr->size() + 1);
			
			Task* task = new Task(name.c_str(), (FUNCPTR)WorkerTask, Task::kDefaultPriority - 1);
			task->Start();
			
			tPtr->push_back(task);
			
			s_availableTasks->Give();
		} // s_tasksMutex
	}
	
	{
		lock_ptr<std::queue<taskStructure> > q(s_taskQ, s_taskQSyncObj);
		
		q->push(task);
	}
	
	s_taskSignal.Give();
}

void TaskPool::Stop()
{
	for(unsigned int i = 0; i < s_tasks.size(); i++)
	{
		s_tasks[i]->Stop();
	}
	
	s_tasks.erase(std::remove_if(s_tasks.begin(), s_tasks.end(), ContainerCleanup::DeleteVector<Task*>), s_tasks.end());
	
	for(unsigned int i = 0; s_tempTasks.size(); i++)
	{
		s_tempTasks[i]->Stop();
	}
	
	s_tempTasks.erase(std::remove_if(s_tempTasks.begin(), s_tempTasks.end(), ContainerCleanup::DeleteVector<Task*>), s_tempTasks.end());

	s_isRunning = false;
	
	R_DELETE(s_availableTasks);
}

INT32 TaskPool::WorkerTemp(taskStructure* t, Task* thisTask)
{
	t->ptr(t->arg0, t->arg1, t->arg2, t->arg3, t->arg4, t->arg5, t->arg6, t->arg7, t->arg8);
	
	for(unsigned int i = 0; i < s_tempTasks.size(); i++)
	{
		if(thisTask == s_tempTasks[i])
		{
			s_tempTasks.erase(s_tempTasks.begin() + i);
			break;
		}
	}
	
	R_DELETE(t);
	R_DELETE(thisTask);
	
	return 0;
}

INT32 TaskPool::WorkerTask()
{
	taskStructure t;
	
	while(true)
	{
		s_taskSignal.Take();
		
		s_availableTasks->Take();
		
		{
			lock_ptr<std::queue<taskStructure> > q(s_taskQ, s_taskQSyncObj);
			
			t = s_taskQ.front();
			s_taskQ.pop();
		}	
		
		t.ptr(t.arg0, t.arg1, t.arg2, t.arg3, t.arg4, t.arg5, t.arg6, t.arg7, t.arg8);
		
		s_availableTasks->Give();
	}
}
