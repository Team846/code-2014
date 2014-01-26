#ifndef RHESUS_RHESUSTASK_H_
#define RHESUS_RHESUSTASK_H_

#include <map>

#include <Task.h>
#include "SyncObject.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	class RhesusTask
	{
	public:
		static RhesusTask& CreateNew(const char* name, FUNCPTR function, INT32 priority = Task::kDefaultPriority, UINT32 stackSize = 20000);
		static void Finalize();
		
		virtual ~RhesusTask();
		
		bool Start(UINT32 arg0 = 0, UINT32 arg1 = 0, UINT32 arg2 = 0, UINT32 arg3 = 0, UINT32 arg4 = 0, 
					UINT32 arg5 = 0, UINT32 arg6 = 0, UINT32 arg7 = 0, UINT32 arg8 = 0);
		
		bool Restart();
		bool Stop();

		bool IsReady();
		bool IsSuspended();

		bool Suspend();
		bool Resume();

		bool Verify();
		
		void Join();

		INT32 GetPriority();
		bool SetPriority(INT32 priority);
		const char* GetName();
		INT32 GetID();
	
	private:
		static std::map<const char*, RhesusTask*> m_tasks;

		static INT32 intern_taskEntry(UINT32 inst, UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, 
				UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8);
		
		static void AddTask(RhesusTask* task);
		static void RemoveTask(const char* name);
		
		RhesusTask(const char* name, FUNCPTR function, INT32 priority = Task::kDefaultPriority, UINT32 stackSize = 20000);
		
		FUNCPTR m_taskFunc;
		
		SyncObject m_taskEndMutex;
		
		Task* task();
		
		Task* m_task;
	};
}
}
}

#endif
