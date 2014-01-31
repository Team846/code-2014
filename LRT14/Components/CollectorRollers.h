#ifndef COLLECTOR_ROLLERS_H_
#define COLLECTOR_ROLLERS_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/CollectorRollersData.h"
#include "../Actuators/LRTTalon.h"

/*!
 * @brief Provides control over the collector rollers.
 */
class CollectorRollers : public Component, public Configurable
{
public:
	CollectorRollers();
	~CollectorRollers();
	
	void OnEnabled(); 
	void OnDisabled();
		
	void UpdateEnabled();
	void UpdateDisabled();
		
	void Configure();
	
private:
	CollectorRollersData* m_rollersData;
	LRTTalon* m_talon;
	float m_forwardSpeed, m_reverseSpeed;
};

#endif
