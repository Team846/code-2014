#ifndef RHESUS_LOCK_ON_H_
#define RHESUS_LOCK_ON_H_

#include "Mutex.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	class lock_on
	{
	public:
		lock_on(Mutex& syncObj)
			: m_syncObj(syncObj)
		{
			m_syncObj.Lock();
		}
		
		~lock_on()
		{
			m_syncObj.Unlock();
		}
		
	private:
		Mutex& m_syncObj;
	};
}
}
}

#endif