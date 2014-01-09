#ifndef RHESUS_ACTION_MANAGER_H_
#define RHESUS_ACTION_MANAGER_H_

#include "IRobotAction.hpp"
#include "../Tasks/BinarySemaphore.h"
#include "../Tasks/lock_on.hpp"
#include "../Tasks/RhesusTask.h"
#include "../Tasks/SyncObject.h"

#include <queue>

using namespace Rhesus::Toolkit::Tasks;

namespace Rhesus
{
namespace Toolkit
{
namespace Action
{
	/*!
	 * @brief Used to execute a series of actions
	 * @author Varun Parthasarathy
	 */
	class ActionManager
	{	
	public:
		
		static void Start();
		
		static void Add(IRobotAction* act);
		
		static void Clear();
		
		static bool isDone();
		
	private:
		
		static void RunAll();

		static std::queue<IRobotAction*> s_actionQueue;
		
		static SyncObject s_so;
		
		static RhesusTask s_task;
		
		static BinarySemaphore s_sem;
		
	};
	
}
}
}

#endif
