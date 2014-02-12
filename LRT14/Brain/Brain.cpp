#include "Brain.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <fstream>
#include <sstream>
#include "../RobotState.h"
#include "../DriverStation/LRTDriverStation.h"
#include "../Config/DriverStationConfig.h"

#include "InputProcessor/DrivetrainInputs.h"
#include "InputProcessor/CollectorRollersInputs.h"
#include "InputProcessor/LauncherLoaderInputs.h"

#include "Automation/Autonomous.h"
#include "Automation/Drive.h"
#include "Automation/Turn.h"
#include "Automation/Collect.h"
#include "Automation/Pass.h"
#include "Automation/Fire.h"
#include "Automation/ChangeLauncherAngle.h"
#include "Automation/Pause.h"
#include "Automation/Parallel.h"
#include "Automation/Sequential.h"
#include "Automation/Repeating.h"

#include "Events/GameModeChangeEvent.h"
#include "Events/JoystickMovedEvent.h"
#include "Events/JoystickPressedEvent.h"
#include "Events/JoystickHeldEvent.h"
#include "Events/JoystickReleasedEvent.h"
#include "Events/MultipleEvent.h"
#include "Events/DelayedEvent.h"

#include <Rhesus/Toolkit/GameState.h>

using namespace std;
using namespace Rhesus::Toolkit;

Brain* Brain::m_instance = NULL;

Brain* Brain::Instance()
{
	if (m_instance == NULL)
		m_instance = new Brain();
	return m_instance;
}

void Brain::Initialize()
{
	if (m_instance == NULL)
		m_instance = new Brain();
}

void Brain::Finalize()
{
	R_DELETE(m_instance);
}

Brain::Brain() :
	Loggable("Brain")
{
	// Create joystick input processors
	m_inputs.push_back(new DrivetrainInputs());
	m_inputs.push_back(new CollectorRollersInputs());
	m_inputs.push_back(new LauncherLoaderInputs());
	
	// Create automation tasks
	Automation* auton = new Autonomous();
	Parallel* positionHold = new Parallel("PositionHold");
	positionHold->AddAutomation(new Drive(0.0));
	positionHold->AddAutomation(new Turn(0.0));
	positionHold->AddAutomation(new Repeating("RepeatPause", new Pause(0)));
	Automation* collect = new Collect();
	Automation* pass = new Pass();
	Automation* fire = new Fire();
	Automation* longShot = new ChangeLauncherAngle(true);
	Automation* shortShot = new ChangeLauncherAngle(false);
	m_automation.push_back(auton);
	m_automation.push_back(positionHold);
	m_automation.push_back(collect);
	m_automation.push_back(fire);
	m_automation.push_back(longShot);
	m_automation.push_back(shortShot);
	
	// Create events to be used
	Event* to_auto = new GameModeChangeEvent(GameState::AUTONOMOUS);
	Event* driver_stick_moved = new JoystickMovedEvent(LRTDriverStation::Instance()->GetDriverStick());
	Event* operator_stick_moved = new JoystickMovedEvent(LRTDriverStation::Instance()->GetOperatorStick());
	Event* driver_stick_pressed = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick());
	Event* operator_stick_pressed = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick());
	Event* disabled_timeout = new DelayedEvent(new GameModeChangeEvent(GameState::DISABLED), 100);
	Event* position_hold_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverWheel(), DriverStationConfig::JoystickButtons::POSITION_HOLD);
	Event* position_hold_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverWheel(), DriverStationConfig::JoystickButtons::POSITION_HOLD);
	Event* collect_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::COLLECT);
	Event* collect_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::COLLECT);
	Event* pass_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::PASS);
	Event* pass_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::PASS);
	Event* fire_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::FIRE);
	Event* fire_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::FIRE);
	Event* long_shot = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::LONG_SHOT);
	Event* short_shot = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::SHORT_SHOT);
	
	// Map events to tasks to start/abort/continue
	to_auto->AddStartListener(auton);
	driver_stick_moved->AddAbortListener(auton);
	operator_stick_moved->AddAbortListener(auton);
	driver_stick_pressed->AddAbortListener(auton);
	operator_stick_pressed->AddAbortListener(auton);
	disabled_timeout->AddAbortListener(auton);
	position_hold_start->AddStartListener(positionHold);
	position_hold_abort->AddAbortListener(positionHold);
	collect_start->AddStartListener(collect);
	collect_abort->AddAbortListener(collect);
	pass_start->AddAbortListener(collect);
	pass_start->AddStartListener(pass);
	pass_abort->AddAbortListener(pass);
	fire_start->AddStartListener(fire);
	fire_abort->AddAbortListener(fire);
	long_shot->AddStartListener(longShot);
	long_shot->AddAbortListener(shortShot);
	short_shot->AddStartListener(shortShot);
	short_shot->AddAbortListener(longShot);
}

Brain::~Brain()
{
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	for (vector<InputProcessor*>::iterator it = m_inputs.begin(); it < m_inputs.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	for (vector<Automation*>::iterator it = Automation::automation_vector.begin(); it < Automation::automation_vector.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
}

void Brain::Update()
{
 	ProcessAutomationTasks();
	
	ProcessInputs();
	
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		(*it)->Update();
	}
}

void Brain::ProcessAutomationTasks()
{
	// Try to start queued tasks
	for (map<Automation*, Event*>::iterator a = m_waitingTasks.begin(); a != m_waitingTasks.end(); a++)
	{
	    if (find(m_runningTasks.begin(), m_runningTasks.end(), a->first) == m_runningTasks.end()) // If task isn't running
	    {
			if (a->first->CheckResources())
			{
				bool ret = a->first->StartAutomation(a->second);
				if (ret)
				{
					m_runningTasks.push_back(a->first);
					m_waitingTasks.erase(a);
				}
			}
        }
	}
	
	// Start/Abort/Continue tasks which had their events fired
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		if ((*it)->Fired())
		{
        	// Tasks aborted by this event
        	for (vector<Automation*>::iterator a = (*it)->GetAbortListeners().begin(); a < (*it)->GetAbortListeners().end(); a++)
        	{
        	    if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) != m_runningTasks.end()) // If task is running
        	    {
        	    	bool ret = (*a)->AbortAutomation(*it);
        		    if (ret)
        		    {
        		    	(*a)->DeallocateResources();
        		        m_runningTasks.remove(*a);
        		    }
        		}
        	}
        	
		    // Tasks started by this event
        	for (vector<Automation*>::iterator a = (*it)->GetStartListeners().begin(); a < (*it)->GetStartListeners().end(); a++)
        	{
        		if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) == m_runningTasks.end() || (*a)->IsRestartable()) // If task isn't running or is restartable
        		{
        			if ((*a)->CheckResources())
        			{
						bool ret = (*a)->StartAutomation(*it);
						if (ret)
							m_runningTasks.push_back(*a);
        			}
        			else
        			{
        				if ((*a)->QueueIfBlocked())
        					m_waitingTasks.insert(pair<Automation*, Event*>(*a, *it));
        			}
        		}
        	}

        	// Tasks continued by this event
        	for (vector<Automation*>::iterator a = (*it)->GetContinueListeners().begin(); a < (*it)->GetContinueListeners().end(); a++)
        	{
        	    if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) != m_runningTasks.end()) // If task is running
        	    {
        		    (*a)->ContinueAutomation(*it);
        		}
        	}
		}
	}
	
    // Update running tasks
    for (list<Automation*>::iterator a = m_runningTasks.begin(); a != m_runningTasks.end(); a++)
    {
    	bool complete = (*a)->Update();
	    if (complete)
	    {
	    	(*a)->DeallocateResources();
	    	m_runningTasks.erase(a++);
	    	a--;
	    }
    }
}

void Brain::ProcessInputs()
{
	for (vector<InputProcessor*>::iterator it = m_inputs.begin(); it < m_inputs.end(); it++)
	{
		if ((*it)->CheckResources())
		{
			(*it)->Update();
		}
	}
}

void Brain::Log()
{
	for (vector<Automation*>::iterator it = m_automation.begin(); it < m_automation.end(); it++)
	{
		LogToFile(find(m_runningTasks.begin(), m_runningTasks.end(), *it) != m_runningTasks.end(), (*it)->GetName());
	}
}
