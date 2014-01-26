#ifndef LAUNCHER_LOADER_H_
#define LAUNCHER_LOADER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/LauncherLoaderData.h"
#include "../Actuators/LRTTalon.h"
#include "DigitalInput.h"

/*!
 * @brief Provides control over launcher loading mechanism.
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
	LauncherLoaderData* m_loaderData;
	LRTTalon* m_talon;
	DigitalInput* m_sensor;
	bool m_firing;
	float m_speed;
};

#endif
