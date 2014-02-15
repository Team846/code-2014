#include "ManagedTask.h"

#include "lock_on.h"

#include "../Defines.h"

using namespace Rhesus::Toolkit::Tasks;

const int ManagedTask::kInvalidTask = -1;
const int ManagedTask::kMaxManagedTasks = 50;
bool ManagedTask::s_isQueueInitialized = false;
std::queue<int> ManagedTask::s_availableIDs;
Mutex ManagedTask::s_idSync;
ManagedTask** ManagedTask::s_managedTasks;

void ManagedTask::Finalize()
{
	for(int i = 0; i < kMaxManagedTasks; i++)
	{
		if(s_managedTasks[i] != NULL)
		{
			R_DELETE(s_managedTasks[i]);
		}
	}
}

ManagedTask::ManagedTask(std::string name, FUNCPTR funptr)
{
	intern_ctor(name, funptr, Task::kDefaultPriority, 20000);
}

ManagedTask::ManagedTask(std::string name, FUNCPTR funptr, INT32 priority, UINT32 stackSize)
{
	intern_ctor(name, funptr, priority, stackSize);
}

ManagedTask::~ManagedTask()
{
	Stop();
	
	releaseTask(this);
	
	R_DELETE(m_task);
	
	m_func = NULL;
}

bool ManagedTask::Start(UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, 
			UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8)
{
	return m_task->Start(reinterpret_cast<UINT32>(this), arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

bool ManagedTask::Stop()
{
	return m_task->Stop();
}

INT32 ManagedTask::releaseTask(ManagedTask* instance)
{
	{
		lock_on l(s_idSync);
		
		int id = instance->m_identifier;
		
		s_managedTasks[id] = NULL;
		
		s_availableIDs.push(id);
		
		instance->m_identifier = kInvalidTask;
	}
	
	return 0;
}

INT32 ManagedTask::registerTask(ManagedTask* instance)
{
	{
		lock_on l(s_idSync);
		
		if(!s_isQueueInitialized)
		{
			s_managedTasks = new ManagedTask*[kMaxManagedTasks];
			
			for(int i = 0; i < kMaxManagedTasks; i++)
			{
				s_availableIDs.push(i);
				s_managedTasks[i] = NULL;
			}
			
			s_isQueueInitialized = true;
		}
		
		if(s_availableIDs.empty())
			return -1;
		
		int id = s_availableIDs.front();
		
		instance->setIdentifier(id);
		s_managedTasks[id] = NULL;
		
		s_availableIDs.pop();
		
		return 0;
	}
}


void ManagedTask::intern_ctor(std::string name, FUNCPTR funptr, INT32 priority, UINT32 stackSize)
{
	bool available = false;
	
	{
		lock_on l(s_idSync);

		available = registerTask(this) == 0;
	}
	
	if(available)
	{
		// Task's ctor copies the name, so we don't have to worry about name going out of scope/dtoring
		m_func = funptr;
		m_task = new Task(name.c_str(), (FUNCPTR)intern_func_wrapper, priority, stackSize);
	}
	else
	{
#ifdef RHESUS_NO_THROW
		// TODO: me
#endif
	}
}

void ManagedTask::setIdentifier(int id)
{
	m_identifier = id;
}
