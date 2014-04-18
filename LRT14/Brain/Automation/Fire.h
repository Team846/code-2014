#ifndef FIRE_H_
#define FIRE_H_

#include "Automation.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/LauncherAngleData.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/BallHolderData.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Fires the ball.
 */
class Fire : public Automation, public Configurable
{
public:
	Fire(bool overrideSensor = false);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
	void Configure();

private:
	LauncherLoaderData* m_loaderData;
	CollectorArmData* m_collectorArmData;
	BallHolderData* m_ballHolder;
	DigitalInput* m_proximity;
//	AnalogChannel* m_proximity;
	Timer m_collectorDownTimer;
	Timer m_fireTimer;
	float m_timeout;
	bool m_hasBall;
	float m_fireTime;
	bool m_firing;
	bool m_override;
	bool m_loaded;
	int m_detection;
};

#endif
