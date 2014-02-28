#include "SynchronizedProcess.h"

SynchronizedProcess::SynchronizedProcess(std::string taskName, INT32 priority)
	: AsyncProcess(taskName, priority)
{
	m_syncSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
}

SynchronizedProcess::SynchronizedProcess(std::string taskName, SEM_ID syncSem, INT32 priority)
	: AsyncProcess(taskName, priority)
{
	m_syncSem = syncSem;
}

SynchronizedProcess::~SynchronizedProcess()
{
	semDelete(m_syncSem);
}

void SynchronizedProcess::RunOneCycle()
{
	semGive(m_syncSem);
}

void SynchronizedProcess::preTick()
{
	semTake(m_syncSem, WAIT_FOREVER);
}
