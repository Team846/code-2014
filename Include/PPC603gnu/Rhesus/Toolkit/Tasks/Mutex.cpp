#include "Mutex.h"

#ifdef __VXWORKS__
#include <semLib.h>
#include <taskLib.h>
#include <sysLib.h>
#endif

using namespace Rhesus::Toolkit::Tasks;

Mutex::Mutex() 
{
#ifdef __VXWORKS__
	m_sem = semMCreate(SEM_Q_PRIORITY);
#endif	
}

Mutex::~Mutex()
{
#ifdef __VXWORKS__
	semDelete(m_sem);
#endif
}

void Mutex::Unlock() 
{
#ifdef __VXWORKS__
	semGive(m_sem);
#endif
}

void Mutex::Lock()
{
	// TODO -1 may not be forever on all platforms
	Lock(-1);
}

void Mutex::Lock(int timeout)
{
#ifdef __VXWORKS__
	semTake(m_sem, timeout);
#endif
}

void Mutex::Flush()
{
#ifdef __VXWORKS__
	semFlush(m_sem);
#endif
}
