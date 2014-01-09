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
		explicit BinarySemaphore(UINT32 state);
		
		void Give();
		void Take();
		void Take(int timeout);
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
