#ifndef LAUNCHER_LOADER_INPUTS_H_
#define LAUNCHER_LOADER_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/CollectorRollersData.h"

/*!
 * @brief Processes joystick information for controlling the launcher loader.
 */
class CollectorRollersInputs : public InputProcessor
{
public:
	CollectorRollersInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	CollectorRollersData* m_rollersData;
};

#endif
