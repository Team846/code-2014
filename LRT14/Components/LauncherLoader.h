#ifndef LAUNCHER_LOADER_H_
#define LAUNCHER_LOADER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/LauncherLoaderData.h"
#include "../Actuators/LRTVictor.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/SensorFactory.h"
#include "../Communication/LiveNetworkSendable.h"

/*!
 * @brief Provides control over launcher loading mechanism, including the safety mechanism.
 */
class LauncherLoader : public Component, public Configurable, public LiveNetworkSendable
{
public:
	LauncherLoader();
	~LauncherLoader();
	
	void OnEnabled();
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	void Send();
	
private:
	LauncherLoaderData* m_loaderData;
	LRTSpeedController* m_motorA;
	LRTSpeedController* m_motorB;
	Pneumatics* m_safety;
	AnalogChannel* m_sensor;
	DigitalInput* m_proximity;
	int m_unloadSetpoint;
	int m_intermediateSetpoint;
	int m_loadSetpoint;
	float m_gain;
	int m_wrapThreshold;
	int m_maxSensorValue;
	int m_completionErrorThreshold;
	
	int m_currentRotation;
	int m_currentSensorValue;
	int m_lastRawSensorValue;
	int m_currentSetpoint;
	bool m_loaded;
};

#endif
