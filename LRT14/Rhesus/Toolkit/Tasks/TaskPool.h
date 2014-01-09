#ifndef RHESUS_TASK_POOL_H_
#define RHESUS_TASK_POOL_H_

#include <queue>
#include <vector>

#include <Task.h> // TaskPool does not use ManagedTasks to make cleanup easier

#include "SyncObject.h"
#include "BinarySemaphore.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{

	class TaskPool
	{
	public:
		static void Start();
		static void Start(INT32 numThreads);
		
		static void EnqueueTask(FUNCPTR ptr, UINT32 arg0 = 0, UINT32 arg1 = 0, UINT32 arg2 = 0, UINT32 arg3 = 0, UINT32 arg4 = 0, 
				UINT32 arg5 = 0, UINT32 arg6 = 0, UINT32 arg7 = 0, UINT32 arg8 = 0);
		
	private:
		struct taskStructure
		{
			FUNCPTR ptr;
			UINT32 arg0;
			UINT32 arg1;
			UINT32 arg2;
			UINT32 arg3;
			UINT32 arg4;
			UINT32 arg5;
			UINT32 arg6;
			UINT32 arg7;
			UINT32 arg8;
		};
		
		static INT32 WorkerTask();
		
		static std::queue<taskStructure> s_taskQ;
		static SyncObject s_taskQSyncObj;
		static BinarySemaphore s_taskSignal;
		static std::vector<Task*> s_tasks;
	};

}
}
}

#endif
