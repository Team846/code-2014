#ifndef COLLECTOR_ARM_DATA_H_
#define COLLECTOR_ARM_DATA_H_

#include "ComponentData.h"

class CollectorArmData : public ComponentData
{
public:
	enum Position
	{
		STOWED,
		COLLECT
	};
	
	CollectorArmData();
	static CollectorArmData* Get();
	
	void SetDesiredPosition(Position position);
	Position GetDesiredPosition();
	
	Position GetCurrentPosition();
	
	void ResetCommands();
	void Log();
	
private:
	void SetCurrentPosition(Position position);
	
	Position m_position;
	Position m_currentPosition;
	
	friend class CollectorArm;
};

#endif
