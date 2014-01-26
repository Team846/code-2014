#include "RhesusTask.h"
#include "../Defines.h"

#include "lock_on.h"

using namespace std;
using namespace Rhesus::Toolkit::Tasks;

map<const char*, RhesusTask*> RhesusTask::m_tasks;

void RhesusTask::AddTask(RhesusTask* task)
{
	m_tasks.insert(std::pair<const char*, RhesusTask*>(task->GetName(), task));
}

void RhesusTask::RemoveTask(const char* name)
{
	if(m_tasks.find(name) == m_tasks.end())
		return;
	
	RhesusTask* task = m_tasks[name];
	m_tasks.erase(name);
	
	DELETE(task);
}

RhesusTask& RhesusTask::CreateNew(const char* name, FUNCPTR function, INT32 priority, UINT32 stackSize)
{
	RhesusTask* task = new RhesusTask(name, function, priority, stackSize);
	
	return *task;
}

void RhesusTask::Finalize()
{
	while(m_tasks.size() > 0)
	{
		RemoveTask(m_tasks.begin()->first);
	}
}

RhesusTask::RhesusTask(const char* name, FUNCPTR function, INT32 priority, UINT32 stackSize)
	: m_taskFunc(function)
{
	m_task = new Task(name, (FUNCPTR)RhesusTask::intern_taskEntry, priority, stackSize);
	
	AddTask(this); // ownership of this instance transferred to the static RhesusTask factory.
}

RhesusTask::~RhesusTask()
{
	Stop();
}

Task* RhesusTask::task()
{
	return m_task;
}

bool RhesusTask::Start(UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, 
			UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8)
{
	return task()->Start(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

bool RhesusTask::Restart()
{
	return task()->Restart();
}

bool RhesusTask::Stop()
{
	RemoveTask(task()->GetName());
	// no point in keeping the task even if Stop() isn't successful; WPILib invalidates the
	// internal task ID regardless and so there isn't anything we can do.
	return task()->Stop();
}

bool RhesusTask::IsReady()
{
	return task()->IsReady();
}

bool RhesusTask::IsSuspended()
{
	return task()->IsSuspended();
}

bool RhesusTask::Suspend()
{
	return task()->Suspend();
}

bool RhesusTask::Resume()
{
	return task()->Restart();
}

bool RhesusTask::Verify()
{
	return task()->Verify();
}

INT32 RhesusTask::GetPriority()
{
	return task()->GetPriority();
}

bool RhesusTask::SetPriority(INT32 priority)
{
	return task()->SetPriority(priority);
}

const char* RhesusTask::GetName()
{
	return task()->GetName();
}

INT32 RhesusTask::GetID()
{
	return task()->GetID();
}

INT32 RhesusTask::intern_taskEntry(UINT32 inst, UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, 
		UINT32 arg5, UINT32 arg6, UINT32 arg7, UINT32 arg8)
{
	RhesusTask* instance = (RhesusTask*)inst;
	
	lock_on l(instance->m_taskEndMutex);
	
	INT32 ret = -1;
	
	try
	{
		ret = instance->m_taskFunc(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	catch(...)
	{ }
	
	RhesusTask::RemoveTask(instance->GetName());
	
	return ret;
}
