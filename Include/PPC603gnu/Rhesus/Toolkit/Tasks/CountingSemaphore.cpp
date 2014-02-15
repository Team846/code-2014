#include "CountingSemaphore.h"

#ifdef __VXWORKS__
#include <semLib.h>
#include <taskLib.h>
#include <sysLib.h>
#endif

using namespace Rhesus::Toolkit::Tasks;

CountingSemaphore::CountingSemaphore(UINT32 initialcount) 
{
#ifdef __VXWORKS__
	m_sem = semCCreate(SEM_Q_PRIORITY, initialcount);
#endif
}

CountingSemaphore::~CountingSemaphore()
{
#ifdef __VXWORKS__
	semDelete(m_sem);
#endif
}

void CountingSemaphore::Give() 
{
#ifdef __VXWORKS__
	semGive(m_sem);
#endif
}

void CountingSemaphore::Take()
{
	// TODO -1 may not be forever on all platforms
	Take(-1);
}

void CountingSemaphore::Take(INT32 timeout)
{
#ifdef __VXWORKS__
	semTake(m_sem, timeout);
#endif
}

bool CountingSemaphore::IsEmpty()
{
#ifdef __VXWORKS__
	bool empty = (semTake(m_sem, WAIT_FOREVER) == ERROR);
	
	if(!empty) semGive(m_sem);
	
	return empty;
#endif
}

void CountingSemaphore::Flush()
{
#ifdef __VXWORKS__
	semFlush(m_sem);
#endif
}
