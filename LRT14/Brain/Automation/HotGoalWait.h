#ifndef HOT_GOAL_WAIT_H_
#define HOT_GOAL_WAIT_H_

#include "Automation.h"
#include "../../Sensors/CheesyVisionServer.h"

#include <Rhesus.Toolkit.Diagnostics.h>

/*!
 * @brief Does nothing for a certain time or number of cycles.
 */
class HotGoalWait : public Automation
{
public:
	HotGoalWait(string side, double timeout=8.0); // 5.0 + 3.0 padding

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
private:
	Rhesus::Toolkit::Diagnostics::Stopwatch m_sw;
	CheesyVisionServer* m_hotGoal;
	double m_timeout;
	string m_side;
};

#endif
