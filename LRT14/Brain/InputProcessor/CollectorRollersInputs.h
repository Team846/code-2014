#ifndef COLLECTOR_ROLLERS_INPUTS_H_
#define COLLECTOR_ROLLERS_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/CollectorRollersData.h"

/*!
 * @brief Processes joystick information for controlling the collector rollers.
 */
class CollectorRollersInputs : public InputProcessor
{
public:
	CollectorRollersInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	DebouncedJoystick* m_driver_stick;
	CollectorRollersData* m_rollersData;
};

#endif
