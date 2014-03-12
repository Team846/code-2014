#ifndef COLLECTOR_ROLLERS_H_
#define COLLECTOR_ROLLERS_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/CollectorRollersData.h"
#include "../Actuators/LRTTalon.h"
#include "../Communication/LiveNetworkSendable.h"
#include "../Sensors/SensorFactory.h"

/*!
 * @brief Provides control over the collector rollers.
 */
class CollectorRollers : public Component, public Configurable, public LiveNetworkSendable
{
public:
	CollectorRollers();
	~CollectorRollers();
	
	void OnEnabled(); 
	void OnDisabled();
		
	void UpdateEnabled();
	void UpdateDisabled();
		
	void Configure();
	void Send();
	
private:
	CollectorRollersData* m_rollersData;
	LRTSpeedController* m_motor;
	GearTooth* m_gearTooth;
};

#endif
