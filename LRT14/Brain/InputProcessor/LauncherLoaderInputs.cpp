#include "LauncherLoaderInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

LauncherLoaderInputs::LauncherLoaderInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_loaderData = LauncherLoaderData::Get();
	
	Dashboard2::AddTelemetryData("Purge Launcher", (INT16)DashboardTelemetryID::LAUNCHER_LOADER_INPUT, DashboardTelemetryType::STRING);
	
	RegisterResource(ControlResource::LAUNCHER_LOADER);
}

void LauncherLoaderInputs::Update()
{
	m_loaderData->SetFire(false);
	m_loaderData->SetLoad(false);
	m_loaderData->SetHairTrigger(false);
	m_loaderData->SetPurge(false);
	
	std::string state = "???";
	
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE_LAUNCHER))
	{
		m_loaderData->SetUnload(true);
		
		state = "PRESSED";
	}
	else
	{
		m_loaderData->SetUnload(false);
		
		state = "RELEASED";
	}
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::FIRE_PREPARE))
	{
		m_loaderData->SetHairTrigger(true);
	}
//	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::SHORT_SHOT) || m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::LONG_SHOT))
//	{
//		m_loaderData->SetFire(true);
//	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_LOADER_INPUT, state);
}
