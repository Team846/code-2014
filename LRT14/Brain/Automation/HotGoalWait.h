#ifndef HOT_GOAL_WAIT_H_
#define HOT_GOAL_WAIT_H_

#include "Automation.h"

#include <Rhesus.Toolkit.Diagnostics.h>

/*!
 * @brief Does nothing for a certain time or number of cycles.
 */
class HotGoalWait : public Automation
{
public:
	HotGoalWait();
	HotGoalWait(double timeout);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
private:
	Rhesus::Toolkit::Diagnostics::Stopwatch m_sw;
	double m_timeout;
};

#endif
