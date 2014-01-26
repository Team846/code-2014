#ifndef RHESUS_LOCK_ON_H_
#define RHESUS_LOCK_ON_H_

#include "SyncObject.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	class lock_on
	{
	public:
		lock_on(SyncObject& syncObj)
			: m_syncObj(syncObj)
		{
			m_syncObj.lock();
		}
		
		~lock_on()
		{
			m_syncObj.unlock();
		}
		
	private:
		SyncObject& m_syncObj;
	};
}
}
}

#endif
