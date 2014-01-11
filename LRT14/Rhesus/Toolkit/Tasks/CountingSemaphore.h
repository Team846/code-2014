#ifndef RHESUS_COUNTINGSEMAPHORE_H_
#define RHESUS_COUNTINGSEMAPHORE_H_

#include "semLib.h"
#include "taskLib.h"
#include "sysLib.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{

	/*!
	 * @brief simple wrapper around semCLib for easier transition to linux
	 * @author Varun Parthasarathy
	 */
	class CountingSemaphore 
	{
	public:
		/*!
		 * @param initialcount the capacity of the semaphore
		 */
		explicit CountingSemaphore(UINT32 initialcount);
		
		/*!
		 * gives the semaphore, increments the count by 1
		 */
		void Give();
		
		/*!
		 * waits forever for an available semaphore
		 */
		void Take();
		
		/*!
		 * waits until the end of the timeout for an available semaphore
		 */
		void Take(INT32 timeout);
		
		/*!
		 * Flushes the semaphore
		 */
		void Flush();
		
	private:
#ifdef __VXWORKS__
		SEM_ID m_sem;
#endif
		
	};

}

}

}

#endif
