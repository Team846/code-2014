#include "ChangeLauncherAngle.h"

ChangeLauncherAngle::ChangeLauncherAngle(bool longShot) :
	Automation("ChangeLauncherAngle")
{
	m_loaderData = LauncherLoaderData::Get();
	m_angleData = LauncherAngleData::Get();
	m_long = longShot;
}

void ChangeLauncherAngle::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::LAUNCHER_ANGLE);
}

bool ChangeLauncherAngle::Start()
{
	return true;
}

bool ChangeLauncherAngle::Run()
{
	if (m_loaderData->IsLoadingComplete())
	{
		m_angleData->SetAngle(m_long ? LauncherAngleData::LONG : LauncherAngleData::SHORT);
		return true;
	}
	return false;
}

bool ChangeLauncherAngle::Abort()
{
	return true;
}

