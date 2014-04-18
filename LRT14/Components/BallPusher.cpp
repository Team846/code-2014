#include "BallPusher.h"

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/Pneumatics.h"

BallPusher::BallPusher() :
	Component("BallPusher", DriverStationConfig::DigitalIns::NO_DS_DI)
{
	m_ballPusherData = BallPusherData::Get();
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/BALL_PUSHER"), "BallPusher");
}

BallPusher::~BallPusher()
{

}

void BallPusher::OnEnabled()
{
	
}
void BallPusher::OnDisabled()
{
	
}
		
void BallPusher::UpdateEnabled()
{
	m_pneumatics->Set(m_ballPusherData->GetPush() ? Pneumatics::FORWARD : Pneumatics::OFF);
}

void BallPusher::UpdateDisabled()
{
	m_pneumatics->Set(Pneumatics::OFF);
}
