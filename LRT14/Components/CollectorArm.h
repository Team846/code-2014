#ifndef COLLECTOR_ARM_H_
#define COLLECTOR_ARM_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/CollectorArmData.h"

/*!
 * @brief Provides control over the collector arm component.
 */
class CollectorArm : public Component, public Configurable
{
public:
	CollectorArm();
	~CollectorArm();
	
	void OnEnabled(); 
	void OnDisabled();
		
	void UpdateEnabled();
	void UpdateDisabled();
		
	void Configure();
};

#endif
