#ifndef COLLECTOR_INPUTS_H_
#define COLLECTOR_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/CollectorData.h"

/*!
 * @brief Processes joystick information for controlling the collector.
 */
class CollectorInputs : public InputProcessor
{
public:
	CollectorInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_driver_stick;
	DebouncedJoystick* m_driver_wheel;
	
	CollectorData* collectorData;
};

#endif
