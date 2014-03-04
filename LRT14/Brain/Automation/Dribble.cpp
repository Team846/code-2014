#include "Dribble.h"
#include <Rhesus/Toolkit/Utilities/MathUtils.h>

using namespace Rhesus::Toolkit::Utilities;

Dribble::Dribble():
		Automation("Dribble"),
		Configurable("Dribble")
{
	m_armData = CollectorArmData::Get();
	m_rollerData = CollectorRollersData::Get();
	m_encoders = DriveEncoders::Get();
	
	Configure();
}

void Dribble::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
}

bool Dribble::Start()
{
	if(m_armData->GetCurrentPosition() == CollectorArmData::STOWED)
		m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
	m_rollerData->SetRunning(true);
	return true;
}

bool Dribble::Run()
{
	CollectorRollersData::Direction direction;
	
	float speed = MathUtils::Clamp(m_encoders->GetNormalizedForwardSpeed() * m_dribbleSpeed, -1.0, 1.0);
	direction = speed > 0 ? CollectorRollersData::FORWARD : CollectorRollersData::REVERSE;
	m_rollerData->SetDirection(direction);
	m_rollerData->SetSpeed(MathUtils::Abs<float>(speed));
	return false;
}

bool Dribble::Abort()
{
	m_rollerData->SetRunning(false);
	m_rollerData->SetSpeed(0.0);
	m_armData->SetDesiredPosition(CollectorArmData::STOWED);
	return true;
}


void Dribble::Configure()
{
	m_dribbleSpeed = GetConfig("dribble_speed", 1.0);
}
