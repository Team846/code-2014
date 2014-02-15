#ifndef RHESUS_COUNTINGSEMAPHORE_H_
#define RHESUS_COUNTINGSEMAPHORE_H_

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
		~CountingSemaphore();
		
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
		 * Checks if this CountingSemaphore is empty.
		 */
		bool IsEmpty();
		
		/*!
		 * Flushes the semaphore
		 */
		void Flush();
		
	private:
#ifdef __VXWORKS__
		SEM_ID m_sem;
#endif
		
		R_DISALLOW_COPY_AND_ASSIGN(CountingSemaphore);
	};

}

}

}

#endif
