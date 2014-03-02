#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include <vector>
#include <string>
#include "Component.h"
#include "../Actuators/LRTVictor.h"
#include "../ComponentData/CollectorData.h"

class Collector : public Component
{
public:

	Collector();
	
	~Collector();
	
protected:

	void UpdateEnabled();
	
	void UpdateDisabled();
	
	void OnEnabled();
	
	void OnDisabled();
	
private:
	
	LRTVictor m_collector; 	
	
	CollectorData* m_collectorData;
};

#endif
