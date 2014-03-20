#ifndef COLLECTOR_INPUTS_H_
#define COLLECTOR_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/CollectorData.h"
#include "../../ComponentData/IndicatorData.h"

/*!
 * @brief Processes joystick information for controlling the collector.
 */
class CollectorInputs : public InputProcessor
{
public:
	CollectorInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	
	CollectorData* collectorData;
	IndicatorData* indicatorData;
};

#endif
