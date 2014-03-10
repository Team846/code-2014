#ifndef COLLECTOR_ARM_H_
#define COLLECTOR_ARM_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../Communication/LiveNetworkSendable.h"
#include "../ComponentData/CollectorArmData.h"
#include <AnalogChannel.h>
#include "../Actuators/Pneumatics.h"
#include <DigitalInput.h>

/*!
 * @brief Provides control over the collector arm component.
 */
class CollectorArm : public Component, public Configurable, public LiveNetworkSendable
{
public:
	CollectorArm();
	~CollectorArm();
	
	void OnEnabled(); 
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	void Send();
	
private:
	CollectorArmData* m_armData;
	Pneumatics* m_pneumatics;
	DigitalInput* m_switch;
};

#endif
