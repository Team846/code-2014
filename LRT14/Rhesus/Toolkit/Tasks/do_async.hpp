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

	template<typename AsyncMethodClassPtr, typename AsyncMethodFuncPtr,
			typename AsyncCallbackClassPtr = AsyncMethodClassPtr,
			typename AsyncCallbackFuncPtr = AsyncMethodFuncPtr>
	class do_async {
	private:
		struct TaskStruct {
			AsyncMethodClassPtr ptr;
			AsyncMethodFuncPtr mptr;
			AsyncCallbackFuncPtr cptr;
			AsyncCallbackClassPtr ccptr;
			UINT32 arg0;
			UINT32 arg1;
			UINT32 arg2;
			UINT32 arg3;
			UINT32 arg4;
			UINT32 arg5;
			UINT32 arg6;
			UINT32 arg7;
			
			INT32 callMethod()
			{
				return (ptr->*mptr)(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			}
			
			INT32 callCallback(INT32 retCode)
			{
				return (ccptr->*cptr)(retCode);
			}
		};
		
	public:
		static INT32 AsyncMethodWrapper(TaskStruct* t) {
			//		printf("!!in wrapper!!\n");
	
			//			do_async* instance = reinterpret_cast<do_async*>(p0);
			//			
			//			if(instance->m_asyncFunc == NULL) return -1;
	
			//		printf("calling func: %u\n", p0);
	
			//call function
			INT32 retCode = t->callMethod();
	
			//callback
			if (t->ccptr != NULL &&t->cptr != NULL)
				t->callCallback(retCode);
			
			DELETE(t);
			
			return 0;
		}
	
		do_async(AsyncMethodClassPtr inst, AsyncMethodFuncPtr asyncFunc,
				UINT32 p0 = 0, UINT32 p1 = 0, UINT32 p2 = 0, UINT32 p3 = 0,
				UINT32 p4 = 0, UINT32 p5 = 0, UINT32 p6 = 0, UINT32 p7 = 0)
		{
			//		printf("!!starting wrapper!!\n");
			TaskStruct* t = create(inst, asyncFunc, NULL, NULL, p0, p1, p2,p3, p4, p5, p6, p7);
			TaskPool::EnqueueTask((FUNCPTR) AsyncMethodWrapper, (UINT32)t);
			//			m_asyncTask->Start((UINT32)this, p0, p1, p2, p3, p4, p5, p6, p7);
		}
	
		do_async(AsyncMethodClassPtr inst, AsyncMethodFuncPtr asyncFunc,
				AsyncMethodClassPtr c_inst, AsyncCallbackFuncPtr callback,
				UINT32 p0 = 0, UINT32 p1 = 0, UINT32 p2 = 0, UINT32 p3 = 0,
				UINT32 p4 = 0, UINT32 p5 = 0, UINT32 p6 = 0, UINT32 p7 = 0)
		{
			//		printf("!!starting wrapper!!\n");
			TaskStruct* t = create(inst, asyncFunc, callback, c_inst, p0, p1, p2,p3, p4, p5, p6, p7);
			TaskPool::EnqueueTask((FUNCPTR) AsyncMethodWrapper, (UINT32)t);
			//			m_asyncTask->Start((UINT32)this, p0, p1, p2, p3, p4, p5, p6, p7);
		}
	
	private:
	
		TaskStruct* create(AsyncMethodClassPtr ptr, AsyncMethodFuncPtr mptr,
				AsyncCallbackFuncPtr cptr, AsyncCallbackClassPtr ccptr, UINT32 arg0, UINT32 arg1, UINT32 arg2,
				UINT32 arg3, UINT32 arg4, UINT32 arg5, UINT32 arg6, UINT32 arg7) 
		{
			TaskStruct* t = new TaskStruct();
			t->ptr = ptr;
			t->mptr = mptr;
			t->cptr = cptr;
			t->ccptr = ccptr;
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
	
		//		INT32 do_func(UINT32 p0, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5,
		//											UINT32 p6, UINT32 p7)
		//		{
		//			(m_asyncInst->*m_asyncFunc)(p0, p1, p2, p3, p4, p5, p6, p7);
		//			
		//			return 0;
		//		}
		//		
		//		void do_callback(INT32 retCode)
		//		{
		//			if(m_callbackInst == NULL || m_asyncCallback == NULL)
		//				return;
		//			
		//			(m_callbackInst->*m_asyncCallback)(retCode);
		//		}
		//		
		//		RhesusTask& m_asyncTask;
		//		
		//		AsyncMethodClassPtr m_asyncInst;
		//		AsyncMethodFuncPtr m_asyncFunc;
		//		AsyncMethodClassPtr m_callbackInst;
		//		AsyncCallbackFuncPtr m_asyncCallback;
	};
}
}
}

#endif
