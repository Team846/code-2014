#ifndef PASS_BACK_H_
#define PASS_BACK_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/BallHolderData.h"
#include "../../Sensors/SensorFactory.h"

/*!
 * @brief Moves into human loading position.
 */
class PassBack : public Automation
{
public:
	PassBack();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	LauncherLoaderData* m_loaderData;
	CollectorArmData* m_collectorArm;
	CollectorRollersData* m_collectorRollers;
	BallHolderData* m_ballHolder;
	DigitalInput* m_proximity;
	bool m_reverse;
};

#endif
