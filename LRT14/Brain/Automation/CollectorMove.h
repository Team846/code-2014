#ifndef COLLECTOR_MOVE_H_
#define COLLECTOR_MOVE_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/PressurePlateData.h"

/*!
 * @brief Puts the collector up or down.
 */
class CollectorMove : public Automation
{
public:
	CollectorMove(bool down);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	CollectorArmData* m_collectorArm;
	PressurePlateData* m_pressurePlate;
	bool m_down;
};

#endif
