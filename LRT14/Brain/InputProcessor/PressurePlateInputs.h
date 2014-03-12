#ifndef PRESSURE_PLATE_INPUTS_H_
#define PRESSURE_PLATE_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/PressurePlateData.h"

/*!
 * @brief Processes joystick information for controlling the pressure plate.
 */
class PressurePlateInputs : public InputProcessor
{
public:
	PressurePlateInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	PressurePlateData* m_pressurePlate;
};

#endif
