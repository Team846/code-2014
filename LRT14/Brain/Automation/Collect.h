#ifndef COLLECT_H_
#define COLLECT_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../Config/Configurable.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Collects the ball onto the bumper and rejects wrong color balls.
 */
class Collect : public Automation, public Configurable
{
public:
	Collect();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
	
private:
	CollectorArmData* m_collectorArm;
	CollectorRollersData* m_collectorRollers;
	DigitalInput* m_proximity;
	AnalogChannel* m_redChannel;
	AnalogChannel* m_greenChannel;
	AnalogChannel* m_blueChannel;
	int m_redBallRed;
	int m_redBallBlue;
	int m_redBallGreen;
	int m_blueBallRed;
	int m_blueBallBlue;
	int m_blueBallGreen;
	int m_colorRange;
	bool m_hasBall;
};

#endif
