#include "BinarySemaphore.h"

#include "SyncObject.h"

#ifdef __VXWORKS__
#include <semLib.h>
#include <taskLib.h>
#include <sysLib.h>
#endif

using namespace Rhesus::Toolkit::Tasks;

BinarySemaphore::BinarySemaphore(UINT32 state) 
{
#ifdef __VXWORKS__
	m_sem = semBCreate(SEM_Q_PRIORITY, (SEM_B_STATE)state);
#endif	
}

BinarySemaphore::BinarySemaphore(int options, UINT32 state)
{
#ifdef __VXWORKS__
	m_sem = semBCreate(options, (SEM_B_STATE)state);
#endif
}

BinarySemaphore::~BinarySemaphore()
{
#ifdef __VXWORKS__
	semDelete(m_sem);
#endif
}

void BinarySemaphore::Give() 
{
#ifdef __VXWORKS__
	semGive(m_sem);
#endif
}

void BinarySemaphore::Take()
{
	Take(SyncObject::TIMEOUT_WAIT_FOREVER);
}

void BinarySemaphore::Take(int timeout)
{
#ifdef __VXWORKS__
	semTake(m_sem, timeout);
#endif
}

void BinarySemaphore::Flush()
{
#ifdef __VXWORKS__
	semFlush(m_sem);
#endif
}
