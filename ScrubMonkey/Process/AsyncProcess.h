#ifndef ASYNC_PROCESS_H_
#define ASYNC_PROCESS_H_

#include <WPILib.h>
#include <sysLib.h>

#include <Rhesus/Toolkit/Defines.h>
#include <Rhesus.Toolkit.Tasks.h>

/*!
 * @brief Base class for asynchronous processes. Asynchronous processes run in a free loop.
 */
class AsyncProcess
{
public:
	AsyncProcess(std::string taskName, INT32 priority=Task::kDefaultPriority);
	virtual ~AsyncProcess();
	
	void Start();
	void Abort(int code=0, double waitSeconds=0);
	
	bool IsRunning();
protected:
	virtual void preTick();
	virtual void Tick() = 0;
	bool m_isRunning;
private:
	static void _TASK_ENTRY(void* asyncProcessInstance);

	Task* m_task;
	
	Rhesus::Toolkit::Tasks::BinarySemaphore m_quittingSignal;
};

#endif
