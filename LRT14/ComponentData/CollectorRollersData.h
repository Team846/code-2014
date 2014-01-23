#ifndef COLLECTOR_ROLLERS_DATA_H_
#define COLLECTOR_ROLLERS_DATA_H_

#include "ComponentData.h"

class CollectorRollersData : public ComponentData
{
public:
	enum Direction
	{
		FORWARD = 1, 
		REVERSE = -1
	};
	
	CollectorRollersData();
	static CollectorRollersData* Get();

	void SetRunning(bool run);
	bool IsRunning();
	
	void SetDirection(Direction direction);
	Direction GetDirection();
	
	void ResetCommands();
	void Log();
	
private:
	bool m_running;
	Direction m_direction;
	
};

#endif
