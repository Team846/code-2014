#ifndef RHESUS_MUTEX_H_
#define RHESUS_MUTEX_H_

#include "semLib.h"
#include "taskLib.h"
#include "sysLib.h"

#include "../Defines.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{

	class Mutex
	{
	public:
		/*!
		 * @brief Creates an instance of Mutex.
		 */
		Mutex();
		~Mutex();
		
		/*!
		 * @brief Unlocks the mutex.
		 */
		void Unlock();
		
		/*!
		 * @brief Waits until the mutex is available.
		 */
		void Lock();
		
		/*!
		 * @brief Waits for the duration of timeout to lock the mutex.
		 */
		void Lock(INT32 timeout);
		
		/*!
		 * @brief Checks if this mutex is locked.
		 */
		bool IsLocked();
		
		/*!
		 * @brief Unblocks all tasks locked on this mutex.
		 */
		void Flush();
		
	private:
#ifdef __VXWORKS__
		SEM_ID m_sem;
#endif
		
		R_DISALLOW_COPY_AND_ASSIGN(Mutex);
	};

}

}

}

#endif
