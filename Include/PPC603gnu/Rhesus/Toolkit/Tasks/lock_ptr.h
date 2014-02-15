#ifndef RHESUS_LOCK_PTR_H_
#define RHESUS_LOCK_PTR_H_

#include "Mutex.h"

#include <WPILib.h>

#include "../Defines.h"

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
		lock_ptr(volatile T& lockObj, Mutex& mutex)
			: m_lockObj(const_cast<T*>(&lockObj)),
			  m_mutex(mutex)
		{
			m_mutex.Lock();
		}
		
		~lock_ptr()
		{
			m_mutex.Unlock();
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
		Mutex& m_mutex;
		
		R_DISALLOW_COPY_AND_ASSIGN(lock_ptr);
	};
}
}
}

#endif
