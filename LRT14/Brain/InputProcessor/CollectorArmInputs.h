#ifndef COLLECTOR_ARM_INPUTS_H_
#define COLLECTOR_ARM_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../Config/Configurable.h"

/*!
 * @brief Processes joystick information for controlling the collector arm.
 */
class CollectorArmInputs : public InputProcessor, public Configurable
{
public:
	CollectorArmInputs();
	
	void Update();

	void Configure();
	
private:
	DebouncedJoystick* m_operator_stick;
	CollectorArmData* m_armData;
	float m_lastSpeed;
	float m_hairTriggerAcceleration;
};

#endif
