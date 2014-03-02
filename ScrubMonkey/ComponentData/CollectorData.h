#ifndef COLLECTOR_DATA_H_
#define COLLECTOR_DATA_H_

#include "ComponentData.h"

class CollectorData : public ComponentData
{
public:
	enum Direction
	{
		PURGE = 1,
		COLLECT = -1,
	};
	
	CollectorData();
	
	void SetEnabled(bool state);
	bool IsEnabled();
	
	void SetDirection(Direction dir);
	Direction GetDirection();
	
	static CollectorData* Get();
	
protected:
	virtual void ResetCommands();

private:
	bool m_state;
	Direction m_direction;
};

#endif
