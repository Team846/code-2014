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
	 * @brief Used to execute a series of actions represented by IRobotAction objects
	 * @author Varun Parthasarathy
	 */
	class ActionManager
	{	
	public:
		
		/*!
		 * Starts the sequence of actions
		 */
		static void Start();
		
		/*!
		 * Adds an action to the queue
		 * @param act The IRobotAction object to add to the back of the queue
		 */
		static void Add(IRobotAction* act);
		
		/*!
		 * Clears the queue of all actions
		 */
		static void Clear();
		
		/*!
		 * @return whether or not the action queue is empty
		 */
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
