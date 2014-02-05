#ifndef LAUNCHER_ANGLE_H_
#define LAUNCHER_ANGLE_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/LauncherAngleData.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/SensorFactory.h"

/*!
 * @brief Provides control over the launch angle changing mechanism.
 */
class LauncherAngle : public Component, public Configurable
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
	LauncherAngleData* m_launcherAngleData;
	Pneumatics* m_pneumatics;
};

#endif
