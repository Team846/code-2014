#ifndef RHESUS_SYNCOBJECT_H_
#define RHESUS_SYNCOBJECT_H_

#ifdef __VXWORKS__
#include <semLib.h>
#include <vxWorks.h>
#endif

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	class SyncObject
	{
	public:
		SyncObject();
		~SyncObject();
		
		void lock();
		bool try_lock(INT32 timeout);
		void unlock();
		
	private:
#ifdef __VXWORKS__
		SEM_ID m_internSem;
#endif
		bool m_locked;
	};
}
}
}

#endif
