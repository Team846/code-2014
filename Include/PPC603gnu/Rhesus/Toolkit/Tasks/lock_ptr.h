#ifndef RHESUS_LOCK_PTR_H_
#define RHESUS_LOCK_PTR_H_

#include "SyncObject.h"

#include <WPILib.h>

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	template<class T>
	class lock_ptr
	{
	public:
		lock_ptr(volatile T& lockObj, SyncObject& mutex)
			: m_lockObj(const_cast<T*>(&lockObj)),
			  m_mutex(mutex)
		{
			m_mutex.lock();
		}
		
		~lock_ptr()
		{
			m_mutex.unlock();
		}
		
		T& operator*()
		{
			return *m_lockObj;
		}
		
		T* operator->()
		{
			return m_lockObj;
		}
		
	private:
		T* m_lockObj;
		SyncObject& m_mutex;
		
		DISALLOW_COPY_AND_ASSIGN(lock_ptr);
	};
}
}
}

#endif
