#include "Collect.h"

Collect::Collect() :
	Automation("Collect", true),
	Configurable("Collect")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
}

void Collect::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
}

bool Collect::Start()
{
	return true;
}

bool Collect::Run()
{
	if (!Aborting())
	{
		m_collectorArm->SetPosition(CollectorArmData::COLLECT);
		m_collectorRollers->SetRunning(true);
		m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
		m_collectorRollers->SetSpeed(1.0);
		return false;
	}
	else
	{
		m_collectorArm->SetPosition(CollectorArmData::STOWED);
		m_collectorRollers->SetRunning(false);
		return true;
	}
}

bool Collect::Abort()
{
	return true;
}

void Collect::Configure()
{

}
