#ifndef COLLECTOR_ARM_INPUTS_H_
#define COLLECTOR_ARM_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/CollectorArmData.h"

/*!
 * @brief Processes joystick information for controlling the collector arm.
 */
class CollectorArmInputs : public InputProcessor
{
public:
	CollectorArmInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	CollectorArmData* m_armData;
};

#endif