#ifndef RHESUS_DO_ASYNC_H_
#define RHESUS_DO_ASYNC_H_

#include "RhesusTask.h"

#include "TaskPool.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	template<typename AsyncMethodClassPtr, typename AsyncMethodFuncPtr, typename AsyncCallbackClassPtr = AsyncMethodClassPtr, typename AsyncCallbackFuncPtr = AsyncMethodFuncPtr>
	class do_async
	{
	public:
		static INT32 AsyncMethodWrapper(UINT32 p0, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5,
											UINT32 p6, UINT32 p7, UINT32 p8)
		{
			printf("!!in wrapper!!\n");
			
			do_async* instance = reinterpret_cast<do_async*>(p0);
			
			if(instance->m_asyncFunc == NULL) return -1;
			
			printf("calling func: %u\n", p0);
			
			INT32 ret = instance->do_func(p1, p2, p3, p4, p5, p6, p7, p8);
			
			instance->do_callback(ret);
			
			return 0;
		}
		do_async(AsyncMethodClassPtr inst, AsyncMethodFuncPtr asyncFunc, UINT32 p0 = 0, UINT32 p1 = 0, UINT32 p2 = 0, UINT32 p3 = 0, UINT32 p4 = 0, UINT32 p5 = 0,
				UINT32 p6 = 0, UINT32 p7 = 0)
			: m_asyncTask(RhesusTask::CreateNew("Async Function", (FUNCPTR)AsyncMethodWrapper)),
			  m_asyncInst(inst),
			  m_asyncFunc(asyncFunc),
			  m_callbackInst(NULL),
			  m_asyncCallback(NULL)
		{
			printf("!!starting wrapper!!\n");
			m_asyncTask.Start((UINT32)this, p0, p1, p2, p3, p4, p5, p6, p7);
		}
		
		do_async(AsyncMethodClassPtr inst, AsyncMethodFuncPtr asyncFunc, AsyncMethodClassPtr c_inst, AsyncCallbackFuncPtr callback, UINT32 p0 = 0, UINT32 p1 = 0, UINT32 p2 = 0, UINT32 p3 = 0, UINT32 p4 = 0, UINT32 p5 = 0,
				UINT32 p6 = 0, UINT32 p7 = 0)
			: m_asyncTask(RhesusTask::CreateNew("Async Function", (FUNCPTR)AsyncMethodWrapper)),
			  m_asyncInst(inst),
			  m_asyncFunc(asyncFunc),
			  m_callbackInst(c_inst),
			  m_asyncCallback(callback)
		{
			printf("!!starting wrapper!!\n");
			TaskPool::EnqueueTask((FUNCPTR)AsyncMethodWrapper, (UINT32)this, p0, p1, p2, p3, p4, p5, p6, p7);
//			m_asyncTask.Start((UINT32)this, p0, p1, p2, p3, p4, p5, p6, p7);
		}
		
	private:
		
		INT32 do_func(UINT32 p0, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5,
											UINT32 p6, UINT32 p7)
		{
			(m_asyncInst->*m_asyncFunc)(p0, p1, p2, p3, p4, p5, p6, p7);
			
			return 0;
		}
		
		void do_callback(INT32 retCode)
		{
			if(m_callbackInst == NULL || m_asyncCallback == NULL)
				return;
			
			(m_callbackInst->*m_asyncCallback)(retCode);
		}
		
		RhesusTask& m_asyncTask;
		
		AsyncMethodClassPtr m_asyncInst;
		AsyncMethodFuncPtr m_asyncFunc;
		AsyncMethodClassPtr m_callbackInst;
		AsyncCallbackFuncPtr m_asyncCallback;
	};
}
}
}

#endif
