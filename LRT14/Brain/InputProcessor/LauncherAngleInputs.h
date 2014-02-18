#ifndef LAUNCHER_ANGLE_INPUTS_H_
#define LAUNCHER_ANGLE_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/LauncherAngleData.h"

/*!
 * @brief Processes joystick information for controlling the launcher angle.
 */
class LauncherAngleInputs : public InputProcessor
{
public:
	LauncherAngleInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	LauncherAngleData* m_angleData;
};

#endif
