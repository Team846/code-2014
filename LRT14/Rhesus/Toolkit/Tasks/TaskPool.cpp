#include "TaskPool.h"

#include <string>

#include "lock_ptr.hpp"

#include "../Utilities/StringUtil.hpp"
#include "../Utilities/ContainerCleanup.hpp"

using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Toolkit::Utilities;

std::queue<TaskPool::taskStructure> TaskPool::s_taskQ;
SyncObject TaskPool::s_taskQSyncObj;
BinarySemaphore TaskPool::s_taskSignal(0);
std::vector<Task*> TaskPool::s_tasks;

void TaskPool::Start()
{
	Start(5);
}

void TaskPool::Start(INT32 numThreads)
{
	for(int i = 0; i < numThreads; i++)
	{
		std::string name = "TaskPool Worker Thread #" + StringUtil::ValToString<int>(i + 1);
		
		Task* task = new Task(name.c_str(), (FUNCPTR)WorkerTask, Task::kDefaultPriority - 1);
		task->Start();
		
		s_tasks.push_back(task);
	}
}

void TaskPool::EnqueueTask(FUNCPTR ptr, UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4,
		UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8)
{
	taskStructure task;
	
	memcpy(&task, &ptr, sizeof(FUNCPTR) + 9 * sizeof(UINT32)); // one-liner!
	
	{
		lock_ptr<std::queue<taskStructure> > q(s_taskQ, s_taskQSyncObj);
		
		q->push(task);
	}
	
	s_taskSignal.Give();
}

// TODO: have a master task distributor thread; if all threads are occupied, create a temporary new one.
INT32 TaskPool::WorkerTask()
{
	std::queue<taskStructure> taskQ;
	
	while(true)
	{
		s_taskSignal.Take();
		
		{
			lock_ptr<std::queue<taskStructure> > q(s_taskQ, s_taskQSyncObj);
			
			while(!q->empty())
			{
				taskStructure t = q->front();
				q->pop();
				
				taskQ.push(t);
			}
		}
		
		while(!taskQ.empty())
		{
			taskStructure t = taskQ.front();
			taskQ.pop();
			
			t.ptr(t.arg0, t.arg1, t.arg2, t.arg3, t.arg4, t.arg5, t.arg6, t.arg7, t.arg8);
		}
		
		ContainerCleanup::ClearQueue(taskQ);
	}
}
