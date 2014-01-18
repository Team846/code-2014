#ifndef COLLECTOR_ARM_H_
#define COLLECTOR_ARM_H_

#include "../Config/Configurable.h"
#include "Component.h"
#include "../Control/PID.h"
#include "../ComponentData/CollectorArmData.h"
#include "../Communication/LiveNetworkSendable.h"

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
	
	
};

#endif
