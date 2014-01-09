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
	m_sem = semCCreate(SEM_Q_FIFO, initialcount);
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
#warning see todo
	Take(-1);
}

void CountingSemaphore::Take(INT32 timeout)
{
#ifdef __VXWORKS__
	semTake(m_sem, timeout);
#endif
}

void CountingSemaphore::Flush()
{
#ifdef __VXWORKS__
	semFlush(m_sem);
#endif
}
