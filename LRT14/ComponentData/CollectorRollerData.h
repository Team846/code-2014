#ifndef COLLECTOR_DATA_H_
#define COLLECTOR_DATA_H_

#include "ComponentData.h"

class CollectorRollerData : public ComponentData
{
public:
	enum RollerDirection
	{
		FORWARD = 1, 
		BACKWARD = -1
	};
	
	CollectorRollerData();
	
	CollectorRollerData* Get();

	void SetEnabled(bool enabled);
	bool GetEnabled();
	
	void SetDirection(RollerDirection r);
	RollerDirection GetDirection();
	
	void ResetCommands();
	void Log();
	
private:
	bool m_enabled;
	RollerDirection m_direction;
	
};

#endif
