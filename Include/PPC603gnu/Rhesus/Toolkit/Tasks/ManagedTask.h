#ifndef RHESUS_MANAGED_TASK_H_
#define RHESUS_MANAGED_TASK_H_

#include <queue>
#include <string>

#include <Task.h>

#include "Mutex.h"

#include "../Defines.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	class ManagedTask
	{
	public:
		static void Finalize();
		
		ManagedTask(std::string name, FUNCPTR funptr);
		ManagedTask(std::string name, FUNCPTR funptr, INT32 priority, UINT32 stackSize);
		~ManagedTask();
		
		bool Start(UINT32 arg0 = 0, UINT32 arg1 = 0, UINT32 arg2 = 0, UINT32 arg3 = 0, UINT32 arg4 = 0, 
				UINT32 arg5 = 0, UINT32 arg6 = 0, UINT32 arg7 = 0, UINT32 arg8 = 0);
		
		bool Stop();
		
	private:
		static const int kInvalidTask;
		static const int kMaxManagedTasks;
		static bool s_isQueueInitialized;
		static std::queue<int> s_availableIDs;
		static Mutex s_idSync;
		static ManagedTask** s_managedTasks;
		
		static INT32 releaseTask(ManagedTask* instance);
		static INT32 registerTask(ManagedTask* instance); // ownership of task instance transferred to manager

		void intern_ctor(std::string name, FUNCPTR funptr, INT32 priority, UINT32 stackSize);
		
		static INT32 intern_func_wrapper(UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, 
			UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8, UINT32 arg9)
		{
			ManagedTask* inst = reinterpret_cast<ManagedTask*>(arg0);
			
			INT32 ret = inst->m_func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			
			return ret;
		}
		
		void setIdentifier(int id);
		
		int m_identifier;
		FUNCPTR m_func;
		Task* m_task;
		
		ManagedTask(ManagedTask& copy)
		{
			m_task = copy.m_task;
			copy.m_task = NULL;
			m_identifier = copy.m_identifier;
			copy.m_identifier = kInvalidTask;
		}
		
		ManagedTask& operator=(const ManagedTask& a)
		{
			ManagedTask& cpy = const_cast<ManagedTask&>(a);
			
			m_task = cpy.m_task;
			cpy.m_task = NULL;
			
			m_identifier = cpy.m_identifier;
			cpy.m_identifier = kInvalidTask;
			
			return *this;
		}
	};
}
}
}

#endif
