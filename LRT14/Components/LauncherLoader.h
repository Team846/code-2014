#ifndef LAUNCHER_LOADER_H_
#define LAUNCHER_LOADER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/LauncherLoaderData.h"
#include "../Actuators/LRTVictor.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/ContinuousPotentiometer.h"
#include "../Communication/LiveNetworkSendable.h"
#include "../Sensors/SensorFactory.h"

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
	void UpdateSensorValues();
	
	LauncherLoaderData* m_loaderData;
	LRTSpeedController* m_motorA;
	LRTSpeedController* m_motorB;
	Pneumatics* m_safety;
	ContinuousPotentiometer* m_sensor;
	float m_unloadSetpoint;
	float m_intermediateSetpoint;
	float m_loadSetpoint;
	float m_hairTriggerSetpoint;
	float m_firePoint;
	float m_gain;
	float m_wrapThreshold;
	float m_completionErrorThreshold;
	float m_desiredZero;
	
	int m_currentRotation;
	float m_currentSensorAngle;
	float m_lastRawSensorAngle;
	float m_currentSetpoint;
	bool m_load;
	int m_desiredRotation;
	bool m_lastFiring;
	float m_maxSpeed;
};

#endif
