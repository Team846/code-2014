#ifndef RHESUS_DO_ASYNC_STATIC_H_
#define RHESUS_DO_ASYNC_STATIC_H_

#include "RhesusTask.h"

#include "TaskPool.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	class do_async_static
	{
	private:
		typedef INT32 (*AsyncMethodFuncPtr)(UINT32 p0, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5,
												UINT32 p6, UINT32 p7);
		
		typedef void (*AsyncCallbackFuncPtr)(INT32 retCode);
		
		struct TaskStruct
		{
			AsyncMethodFuncPtr mptr;
			AsyncCallbackFuncPtr cptr;
			UINT32 arg0;
			UINT32 arg1;
			UINT32 arg2;
			UINT32 arg3;
			UINT32 arg4;
			UINT32 arg5;
			UINT32 arg6;
			UINT32 arg7;
		};
		
	public:
		static INT32 AsyncMethodWrapper(TaskStruct* t)
		{
			INT32 retCode = (t->mptr)(t->arg0, t->arg1, t->arg2, t->arg3, t->arg4, t->arg5, t->arg6, t->arg7);

			//callback
			if (t->cptr != NULL)
				(t->cptr)(retCode);

			DELETE(t);

			return 0;
		}
		
		do_async_static(AsyncMethodFuncPtr asyncFunc, AsyncCallbackFuncPtr callback = NULL, UINT32 p0 = 0, UINT32 p1 = 0, UINT32 p2 = 0, UINT32 p3 = 0, UINT32 p4 = 0, UINT32 p5 = 0,
				UINT32 p6 = 0, UINT32 p7 = 0)
		{
			TaskPool::EnqueueTask((FUNCPTR)AsyncMethodWrapper, (UINT32)create(asyncFunc, callback, p0, p1, p2, p3, p4, p5, p6, p7));
		}
		
	private:
		TaskStruct* create(AsyncMethodFuncPtr mptr, AsyncCallbackFuncPtr cptr, UINT32 arg0, UINT32 arg1, UINT32 arg2,
				UINT32 arg3, UINT32 arg4, UINT32 arg5, UINT32 arg6, UINT32 arg7) 
		{
			TaskStruct* t = new TaskStruct();
			t->mptr = mptr;
			t->cptr = cptr;
			t->arg0 = arg0;
			t->arg1 = arg1;
			t->arg2 = arg2;
			t->arg3 = arg3;
			t->arg4 = arg4;
			t->arg5 = arg5;
			t->arg6 = arg6;
			t->arg7 = arg7;
			return t;
		}
		
//		RhesusTask& m_asyncTask;
//		
//		AsyncMethodFuncPtr m_asyncFunc;
//		AsyncCallbackFuncPtr m_asyncCallback;
	};
}
}
}

#endif
