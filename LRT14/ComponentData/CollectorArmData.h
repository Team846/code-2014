#ifndef COLLECTOR_ARM_DATA_H_
#define COLLECTOR_ARM_DATA_H_

#include "ComponentData.h"

/*!
 * @brief Contains commands and states for the collector arm component.
 */
class CollectorArmData : public ComponentData
{
public:
	enum Position
	{
		STOWED,
		COLLECT,
		INTERMEDIATE
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
