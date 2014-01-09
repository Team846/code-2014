#include "SyncObject.h"

using namespace Rhesus::Toolkit::Tasks;

SyncObject::SyncObject()
	: m_locked(false)
{
#ifdef __VXWORKS__
	m_internSem = semMCreate(SEM_Q_PRIORITY | SEM_Q_FIFO | SEM_DELETE_SAFE);
#endif
}

SyncObject::~SyncObject()
{
#ifdef __VXWORKS__
	semDelete(m_internSem);
#endif
}

void SyncObject::lock()
{
#ifdef __VXWORKS__
	STATUS stat = semTake(m_internSem, WAIT_FOREVER);
	
	if(stat == OK)
		m_locked = true;
#endif
}

bool SyncObject::try_lock(INT32 timeout)
{
#ifdef __VXWORKS__
	STATUS stat = semTake(m_internSem, timeout);
	
	if(stat == OK) m_locked = true;
	
	return stat == OK;
#endif

}

void SyncObject::unlock()
{
#ifdef __VXWORKS__
	semDelete(m_internSem);
#endif
	m_locked = false;
}
