#ifndef COLLECT_H_
#define COLLECT_H_

#include "Automation.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../Config/Configurable.h"

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
	CollectorArmData *m_collectorArm;
	CollectorRollersData *m_collectorRollers;
};

#endif
