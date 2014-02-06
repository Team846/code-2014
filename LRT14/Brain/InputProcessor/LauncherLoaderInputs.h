#ifndef LAUNCHER_LOADER_INPUTS_H_
#define LAUNCHER_LOADER_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/LauncherLoaderData.h"

/*!
 * @brief Processes joystick information for controlling the launcher loader.
 */
class LauncherLoaderInputs : public InputProcessor
{
public:
	LauncherLoaderInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	LauncherLoaderData* m_loaderData;
};

#endif
