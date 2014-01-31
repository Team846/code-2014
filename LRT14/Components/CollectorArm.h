#ifndef COLLECTOR_ARM_H_
#define COLLECTOR_ARM_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/CollectorArmData.h"
#include <AnalogChannel.h>
#include "../Actuators/LRTTalon.h"

/*!
 * @brief Provides control over the collector arm component.
 */
class CollectorArm : public Component, public Configurable
{
public:
	CollectorArm();
	~CollectorArm();
	
	float ComputeOutput(int setpoint);
	
	void OnEnabled(); 
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	
private:
	LRTTalon *m_talon;
	AnalogChannel *m_pot;
	CollectorArmData *m_armData;
	int m_collectSetpoint, m_stowedSetpoint, m_maxPotValue;
};

#endif
