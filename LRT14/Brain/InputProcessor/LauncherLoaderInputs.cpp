#include "LauncherLoaderInputs.h"
#include "../../Config/RobotConfig.h"

LauncherLoaderInputs::LauncherLoaderInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_loaderData = LauncherLoaderData::Get();
	RegisterResource(ControlResource::LAUNCHER_LOADER);
}

void LauncherLoaderInputs::Update()
{
	m_loaderData->SetFire(false);
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE_LAUNCHER) || m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::HUMAN_LOAD))
	{
		m_loaderData->SetPurge(true);
	}
	else
	{
		m_loaderData->SetPurge(false);
	}
}
