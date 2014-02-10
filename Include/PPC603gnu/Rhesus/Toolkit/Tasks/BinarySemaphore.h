#ifndef RHESUS_BINARYSEMAPHORE_H_
#define RHESUS_BINARYSEMAPHORE_H_

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
	 * @brief simple wrapper around semBLib for easier transition to linux
	 * @author Varun Parthasarathy
	 */
	class BinarySemaphore 
	{	
	public:
		/*!
		 * @param state initial state of the semaphore
		 */
		explicit BinarySemaphore(UINT32 state);
		~BinarySemaphore();
		
		BinarySemaphore(int options, UINT32 state);
		
		explicit BinarySemaphore(SEM_ID s);
		
		/*!
		 * Gives the semaphore
		 */
		void Give();
		
		/*!
		 * Waits forever for a semaphore to be given, and takes it
		 */
		void Take();
		
		/*!
		 * Waits until the timeout expires or a semaphore is given
		 * @param timeout the number of milliseconds to wait before continuing
		 */
		void Take(int timeout);
		
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
