#include "BallHolder.h"

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/Pneumatics.h"

BallHolder::BallHolder() :
	Component("BallHolder", DriverStationConfig::DigitalIns::BALL_HOLDER),
	Configurable("BallHolder")
{
	m_ballHolderData = BallHolderData::Get();
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/BALL_HOLDER"), "BallHolder");
}

BallHolder::~BallHolder()
{

}

void BallHolder::OnEnabled()
{
	
}
void BallHolder::OnDisabled()
{
	
}
		
void BallHolder::UpdateEnabled()
{
	Pneumatics::State state = Pneumatics::OFF;
	std::string stateStr = "OFF";
	
	if (m_ballHolderData->GetHold())
	{
		state = Pneumatics::FORWARD;
		stateStr = "OFF";
	}
	else
	{
		state = Pneumatics::OFF;
		stateStr = "FORWARD";
	}
	m_pneumatics->Set(state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::BALL_HOLDER_STATE, stateStr);
}

void BallHolder::UpdateDisabled()
{
	std::string stateStr = "OFF";
	m_pneumatics->Set(Pneumatics::OFF);
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::BALL_HOLDER_STATE, stateStr);
}

void BallHolder::Configure()
{
	
}
