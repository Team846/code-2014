#ifndef PRESSURE_PLATE_DATA_H_
#define PRESSURE_PLATE_DATA_H_

#include "ComponentData.h"

/*!
 * @brief Contains commands and states for the pressure plate component.
 */
class PressurePlateData : public ComponentData
{
public:
	PressurePlateData();
	static PressurePlateData* Get();
	
	bool GetPressure();
	void SetPressure(bool apply);
	
	void ResetCommands();
	void Log();
	
private:
	bool m_applyPressure;
	
	friend class PressurePlate;
};

#endif

