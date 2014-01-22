#ifndef COLLECTOR_DATA_H_
#define COLLECTOR_DATA_H_

#include "ComponentData.h"

class CollectorRollerData : public ComponentData
{
public:
	enum Direction
	{
		FORWARD = 1, 
		REVERSE = -1
	};
	
	CollectorRollerData();
	CollectorRollerData* Get();

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
