#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include "../Config/Configurable.h"
#include "Component.h"
#include "../Control/PID.h"
#include "../ComponentData/CollectorRollerData.h"
#include "../Communication/LiveNetworkSendable.h"

/*!
 * @brief Provides control over the collector roller component.
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
	
	
};

#endif
