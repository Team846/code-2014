#ifndef LAUNCHER_LOADER_H_
#define LAUNCHER_LOADER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/LauncherLoaderData.h"
#include "../Actuators/LRTVictor.h"
#include "../Sensors/SensorFactory.h"

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
	LRTVictor* m_victorA;
	LRTVictor* m_victorB;
	AnalogChannel* m_sensor;
	bool m_firing;
	float m_forwardSpeed;
	float m_reverseSpeed;
	float setpoint;
	float m_gain;
	float m_closed_loop_threshold;
	int m_completionErrorThreshold;
};

#endif
