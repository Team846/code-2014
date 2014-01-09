#ifndef RHESUS_DO_ASYNC_STATIC_H_
#define RHESUS_DO_ASYNC_STATIC_H_

#include "RhesusTask.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	typedef INT32 (*AsyncMethodFuncPtr)(UINT32 p0, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5,
											UINT32 p6, UINT32 p7);
	
	typedef void (*AsyncCallbackFuncPtr)(INT32 retCode);

	class do_async_static
	{
	public:
		static INT32 AsyncMethodWrapper(UINT32 p0, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5,
											UINT32 p6, UINT32 p7, UINT32 p8)
		{
			do_async_static* instance = reinterpret_cast<do_async_static*>(p0);
			
			if(instance->m_asyncFunc == NULL) return -1;
			
			INT32 ret = instance->m_asyncFunc(p1, p2, p3, p4, p5, p6, p7, p8);
			
			if(instance->m_asyncCallback != NULL)
				instance->m_asyncCallback(ret);
			
			return 0;
		}
		
		do_async_static(AsyncMethodFuncPtr asyncFunc, AsyncCallbackFuncPtr callback, UINT32 p0 = 0, UINT32 p1 = 0, UINT32 p2 = 0, UINT32 p3 = 0, UINT32 p4 = 0, UINT32 p5 = 0,
				UINT32 p6 = 0, UINT32 p7 = 0)
			: m_asyncTask(RhesusTask::CreateNew("Async Function", (FUNCPTR)AsyncMethodWrapper))
		{
			m_asyncTask.Start((UINT32)this, p0, p1, p2, p3, p4, p5, p6, p7);
			
			m_asyncFunc = asyncFunc;
			m_asyncCallback = callback;
		}
		
	private:
		RhesusTask& m_asyncTask;
		
		AsyncMethodFuncPtr m_asyncFunc;
		AsyncCallbackFuncPtr m_asyncCallback;
	};
}
}
}

#endif
