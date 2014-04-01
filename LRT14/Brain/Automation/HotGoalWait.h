#ifndef HOT_GOAL_WAIT_H_
#define HOT_GOAL_WAIT_H_

#include "Automation.h"

/*!
 * @brief Does nothing for a certain time or number of cycles.
 */
class HotGoalWait : public Automation
{
public:
	HotGoalWait();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
};

#endif
