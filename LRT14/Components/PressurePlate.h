#ifndef LAUNCHER_ANGLE_H_
#define LAUNCHER_ANGLE_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/PressurePlateData.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/SensorFactory.h"

/*!
 * @brief Provides control over the launch angle changing mechanism.
 */
class PressurePlate : public Component, public Configurable
{
public:
	PressurePlate();
	~PressurePlate();
	
	void OnEnabled();
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	
private:
	PressurePlateData* m_pressurePlateData;
	Pneumatics* m_pneumatics;
};

#endif
