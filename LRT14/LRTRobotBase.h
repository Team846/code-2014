#ifndef LRT_ROBOT_BASE_
#define LRT_ROBOT_BASE_

#include <WPILib.h>
#include <semLib.h>

class LRTRobotBase : public RobotBase
{
public:
	LRTRobotBase();
	virtual ~LRTRobotBase();
	
	virtual void RobotInit() = 0;
	
	// Inherited from RobotBase, starts the competition
	virtual void StartCompetition();
	
	// Runs once every tick.
	virtual void Tick();

private:
	void Run();

	Notifier m_loopSynchronizer;
	static bool m_isRunning;

	static void ReleaseLoop(void * param);
	
	SEM_ID m_loopSynchSem;
	bool m_robotMainOverridden;
};

#endif
