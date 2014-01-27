#ifndef LAUNCHER_ANGLE_H_
#define LAUNCHER_ANGLE_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/LauncherAngleData.h"
#include "../Actuators/LRTTalon.h"
#include <AnalogChannel.h>

class LauncherAngle :  public Component, public Configurable
{
public:
	LauncherAngle();
	~LauncherAngle();
		
	void OnEnabled(); 
	void OnDisabled();
			
	void UpdateEnabled();
	void UpdateDisabled();
			
	void Configure();
	
private:
	LauncherAngleData *m_launcherAngleData;
	LRTTalon *m_talon;
	AnalogChannel *analogChannel;
	float m_lowSetpoint, m_highSetpoint;
	float m_gain;
};

#endif
