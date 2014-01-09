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
		explicit CountingSemaphore(UINT32 initialcount);
		
		void Give();
		void Take();
		void Take(INT32 timeout);
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
