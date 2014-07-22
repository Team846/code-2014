#ifndef HUMAN_LOAD_H_
#define HUMAN_LOAD_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/BallHolderData.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Moves into human loading position.
 */
class HumanLoad : public Automation
{
public:
	HumanLoad();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	CollectorArmData* m_collectorArm;
	BallHolderData* m_ballHolder;
	DigitalInput* m_proximity;
};

#endif
