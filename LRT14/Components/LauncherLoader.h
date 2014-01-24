#ifndef LAUNCHER_LOADER_H_
#define LAUNCHER_LOADER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../Actuators/LRTTalon.h"
#include "../ComponentData/LauncherLoaderData.h"
#include "DigitalInput.h"

/*!
 * @brief Provides control over luancher loader
 */

class LauncherLoader : public Component, public Configurable
{
public:
	LauncherLoader();
	~LauncherLoader();
	
	void OnEnabled(); 
	void OnDisabled();
			
	void UpdateEnabled();
	void UpdateDisabled();
			
	void Configure();
	
private:
	LRTTalon* m_talon;
	LauncherLoaderData* m_loaderData;
	DigitalInput* m_sensor;
	bool m_launcherLoaderData;
	bool m_firing;
	float m_speed;
};

#endif
