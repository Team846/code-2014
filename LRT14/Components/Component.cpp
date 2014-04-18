#include "Component.h"
#include "Drivetrain.h"
#include "CollectorArm.h"
#include "CollectorRollers.h"
#include "LauncherLoader.h"
#include "LauncherAngle.h"
#include "BallHolder.h"
#include "../RobotState.h"
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

#include <Rhesus/Toolkit/GameState.h>

using namespace std;
using namespace Rhesus::Toolkit;

vector<Component*> Component::component_vector;

Component::Component(std::string name, int driverStationDigitalIn) :
	m_name(name), m_digitalIn(driverStationDigitalIn)
{
	m_lastEnabled = false;
	
	BufferedConsole::Printf("Created component: %s\n", name.c_str());
}

Component::~Component()
{
}

void Component::CreateComponents()
{
	component_vector.push_back(new Drivetrain());
	component_vector.push_back(new CollectorArm());
	component_vector.push_back(new CollectorRollers());
	component_vector.push_back(new LauncherLoader());
	component_vector.push_back(new LauncherAngle());
	component_vector.push_back(new BallHolder());
}

void Component::DestroyComponents()
{
	for (vector<Component*>::iterator it = component_vector.begin(); it < component_vector.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	component_vector.clear();
}

void Component::UpdateAll()
{
	for (vector<Component*>::iterator it = component_vector.begin(); it < component_vector.end(); it++)
	{
		(*it)->Update();
	}
}

void Component::Update()
{
	if (RobotState::Instance().GameMode() != GameState::DISABLED)
	{
		if (m_digitalIn == -1 || DriverStation::GetInstance()->GetDigitalIn(m_digitalIn))
		{
			if (!m_lastEnabled)
				OnEnabled();
			UpdateEnabled();
			m_lastEnabled = true;
		}
		else
		{
			if (m_lastEnabled)
				OnDisabled();
			UpdateDisabled();
			m_lastEnabled = false;
		}
	}
	else
	{
		if (m_lastEnabled)
			OnDisabled();
		UpdateDisabled();
		m_lastEnabled = false;
	}
}

std::string Component::GetName()
{
	return m_name;
}

int Component::GetDigitalIn()
{
	return m_digitalIn;
}

