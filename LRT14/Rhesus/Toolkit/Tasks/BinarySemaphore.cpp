#include "BinarySemaphore.h"

#ifdef __VXWORKS__
#include <semLib.h>
#include <taskLib.h>
#include <sysLib.h>
#endif

using namespace Rhesus::Toolkit::Tasks;

BinarySemaphore::BinarySemaphore(UINT32 state) 
{
#ifdef __VXWORKS__
	m_sem = semBCreate(SEM_Q_FIFO, (SEM_B_STATE)state);
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
	// TODO -1 may not be forever on all platforms
	Take(-1);
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
