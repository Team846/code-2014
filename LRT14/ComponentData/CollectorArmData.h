#ifndef COLLECTOR_ARM_DATA_H_
#define COLLECTOR_ARM_DATA_H_

#include "ComponentData.h"

class CollectorArmData : public ComponentData
{
public:
	enum Position
	{
		STOWED,
		HOLD,
		PURGE,
		COLLECT
	};
	
	CollectorArmData();
	CollectorArmData* Get();
	
	void SetPosition(Position position);
	Position GetPosition();
	
	void ResetCommands();
	void Log();
	
private:
	Position m_position;
};

#endif
