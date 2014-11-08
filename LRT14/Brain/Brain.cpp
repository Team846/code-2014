#include "Brain.h"
#include <fstream>
#include <sstream>
#include "../RobotState.h"
#include "../DriverStation/LRTDriverStation.h"
#include "../Config/DriverStationConfig.h"

#include "InputProcessor/DrivetrainInputs.h"
#include "InputProcessor/CollectorArmInputs.h"
#include "InputProcessor/CollectorRollersInputs.h"
#include "InputProcessor/LauncherLoaderInputs.h"
#include "InputProcessor/LauncherAngleInputs.h"
#include "InputProcessor/BallHolderInputs.h"

#include "Automation/Autonomous.h"
#include "Automation/Drive.h"
#include "Automation/Turn.h"
#include "Automation/Collect.h"
#include "Automation/Pass.h"
#include "Automation/Fire.h"
#include "Automation/LoadLauncher.h"
#include "Automation/PurgeLauncher.h"
#include "Automation/HumanLoad.h"
#include "Automation/PassBack.h"
#include "Automation/Dribble.h"
#include "Automation/Pause.h"
#include "Automation/Parallel.h"
#include "Automation/Sequential.h"
#include "Automation/Repeating.h"
#include "Automation/LuaScript.h"
#include "Automation/TurnJoystickDirection.h"

#include "Events/GameModeChangeEvent.h"
#include "Events/JoystickMovedEvent.h"
#include "Events/JoystickPressedEvent.h"
#include "Events/JoystickHeldEvent.h"
#include "Events/JoystickReleasedEvent.h"
#include "Events/MultipleEvent.h"
#include "Events/DelayedEvent.h"

#include <Rhesus/Toolkit/GameState.h>
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;
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
	m_inputs.push_back(new DrivetrainInputs(DrivetrainInputs::DRIVE));
	m_inputs.push_back(new DrivetrainInputs(DrivetrainInputs::TURN));
	m_inputs.push_back(new CollectorArmInputs());
	m_inputs.push_back(new CollectorRollersInputs());
	m_inputs.push_back(new LauncherLoaderInputs());
	m_inputs.push_back(new LauncherAngleInputs());
	m_inputs.push_back(new BallHolderInputs());
	
	// Create automation tasks
	Automation* auton = new Autonomous();
	Parallel* positionHold = new Parallel("PositionHold");
	positionHold->AddAutomation(new Drive(0.0));
	positionHold->AddAutomation(new Turn(0.0));
	positionHold->AddAutomation(new Repeating("RepeatPause", new Pause(0)));
	Automation* collect = new Collect();
	Automation* pass = new Pass();
	Automation* fire = new Fire();
	Automation* luaScript = new LuaScript("Script.lua");
	Automation* load = new LoadLauncher();
	Automation* purge = new PurgeLauncher();
	Automation* humanLoad = new HumanLoad();
	Automation* dribble = new Dribble();
	Automation* passBack = new PassBack();
	Parallel* turn90 = new Parallel("Turn90");
	turn90->AddAutomation(new TurnJoystickDirection(90, 0.5));
	turn90->AddAutomation(new Repeating("RepeatPause", new Pause(0)));
	Parallel* turn180 = new Parallel("Turn180");
	turn180->AddAutomation(new TurnJoystickDirection(180, 0.5));
	turn180->AddAutomation(new Repeating("RepeatPause", new Pause(0)));
	m_automation.push_back(auton);
	m_automation.push_back(positionHold);
	m_automation.push_back(collect);
	m_automation.push_back(fire);
	m_automation.push_back(load);
	m_automation.push_back(purge);
	m_automation.push_back(humanLoad);
	m_automation.push_back(dribble);
	m_automation.push_back(passBack);
	m_automation.push_back(luaScript);
	m_automation.push_back(turn90);
	m_automation.push_back(turn180);
	
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
	Event* pass_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::PASS);
	Event* pass_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::PASS);
	Event* fire_start_long = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::LONG_SHOT);
	Event* fire_start_short = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::SHORT_SHOT);
	MultipleEvent* fire_abort = new MultipleEvent();
	fire_abort->AddEvent(new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::LONG_SHOT));
	fire_abort->AddEvent(new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::SHORT_SHOT));
	Event* load_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::LOAD_LAUNCHER);
	Event* load_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::LOAD_LAUNCHER);
	Event* purge_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::PURGE_LAUNCHER);
	Event* purge_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::PURGE_LAUNCHER);
	Event* human_load_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::HUMAN_LOAD);
	Event* human_load_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::HUMAN_LOAD);
	Event* dribble_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::DRIBBLE);
	Event* dribble_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::DRIBBLE);
	Event* pass_back_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::PASS_BACK);
	Event* pass_back_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::PASS_BACK);
	Event* turn90_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::TURN_90);
	Event* turn90_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::TURN_90);
	Event* turn180_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::TURN_180);
	Event* turn180_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::TURN_180);
//	Event* lua_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::LUA_SCRIPT);
//	Event* lua_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::LUA_SCRIPT);
	
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
	collect_start->AddAbortListener(dribble);
	collect_abort->AddAbortListener(collect);
	dribble_start->AddAbortListener(collect);
	pass_start->AddAbortListener(collect);
//	pass_start->AddAbortListener(humanLoad);
	pass_start->AddAbortListener(load);
	pass_start->AddStartListener(pass);
	pass_abort->AddAbortListener(pass);
	fire_start_long->AddStartListener(fire);
	fire_start_short->AddStartListener(fire);
	fire_abort->AddAbortListener(fire);
	load_start->AddStartListener(load);
	load_abort->AddAbortListener(load);
	load_start->AddAbortListener(collect);
//	load_start->AddAbortListener(humanLoad);
	purge_start->AddStartListener(purge);
	purge_start->AddAbortListener(collect);
	purge_abort->AddAbortListener(purge);
	human_load_start->AddStartListener(humanLoad);
	human_load_abort->AddAbortListener(humanLoad);
	pass_back_start->AddStartListener(passBack);
	pass_back_abort->AddAbortListener(passBack);
	turn90_start->AddStartListener(turn90);
	turn90_abort->AddAbortListener(turn90);
	turn180_start->AddStartListener(turn180);
	turn180_abort->AddAbortListener(turn180);
//	dribble_start->AddStartListener(dribble);
	dribble_abort->AddAbortListener(dribble);
//	lua_start->AddStartListener(luaScript);
//	lua_abort->AddAbortListener(luaScript);
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

void Brain::PrintRunningAutomation()
{
	BufferedConsole::Printf("Running Automation Routines:\n");
	for (vector<Automation*>::iterator it = m_automation.begin(); it < m_automation.end(); it++)
	{
		if (find(m_runningTasks.begin(), m_runningTasks.end(), *it) != m_runningTasks.end())
			BufferedConsole::Printf("%s\n", (*it)->GetName().c_str());
	}
}
