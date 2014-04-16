#include "LuaScript.h"

#include "Drive.h"
#include "Turn.h"
#include "JitterTurn.h"
#include "Fire.h"
#include "Dribble.h"
#include "LoadLauncher.h"
#include "Parallel.h"
#include "Sequential.h"
#include "Pause.h"
#include "../../Sensors/SensorFactory.h"

#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Scripting;

LuaScript::LuaScript(std::string file)
	: Automation("LuaScript"), m_scriptProvider(), m_currentRoutine(NULL), m_error(false), m_resources()
{
	m_scriptProvider.CreateContext();
	lua_State* L = m_scriptProvider.L();
	
	lua_getglobal(L, "AllocateResources");
	
	if(lua_pcall(L, 0, 0, 0) != 0)
	{
		m_error = true;
		return;
	}
	
	if(lua_gettop(L) != 1 || !lua_istable(L, 1))
	{
		m_error = true;
		return;
	}
	
	// for each entry in the table
	int len = lua_rawlen(L, 1);

	for (int i = 1; i <= len; i++)
	{
		// get the entry to stack
		lua_pushinteger(L, i);
		lua_gettable(L, 1);

		// get table entry as string
		const char *s = lua_tostring(L, -1);
		if (s)
		{
			// push the value to the vector
			m_resources.push_back(s);
		}

		// remove entry from stack
		lua_pop(L,1);
	}
}

LuaScript::~LuaScript()
{
	delete m_currentRoutine;
	m_currentRoutine = NULL;
}

bool LuaScript::Start()
{
	lua_State* L = m_scriptProvider.L();
	
	lua_pushinteger(L, reinterpret_cast<size_t>(this));
	lua_setglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	
	m_scriptProvider.ExposeEntity("drive", (lua_CFunction)lua_Drive);
	m_scriptProvider.ExposeEntity("turn", (lua_CFunction)lua_Turn);
	m_scriptProvider.ExposeEntity("jitterTurn", (lua_CFunction)lua_JitterTurn);
	m_scriptProvider.ExposeEntity("fire", (lua_CFunction)lua_Turn);
	m_scriptProvider.ExposeEntity("dribbleDrive", (lua_CFunction)lua_DribbleDrive);
	m_scriptProvider.ExposeEntity("loadLauncher", (lua_CFunction)lua_LoadLauncher);
	m_scriptProvider.ExposeEntity("getHotLeftStatus", (lua_CFunction)lua_HotLeftStatus);
	m_scriptProvider.ExposeEntity("getHotRightStatus", (lua_CFunction)lua_HotRightStatus);
	
	return true;
}

int LuaScript::lua_Drive(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	double args[3];
	
	for(int i = 0; i < n && i < 3; i++) // i < 3 because overload with most params is drive(double, double, double, bool)
	{
		if(!lua_isnumber(L, i + 1)) // +1 because lua is 1-based
		{
			error = true;
			break;
		}
		
		args[i] = lua_tonumber(L, i + 1);
	}
	
	if(!error)
	{
		if(n == 1)
		{
			inst->m_currentRoutine = new Drive(args[0]);
		}
		else if(n == 2)
		{
			inst->m_currentRoutine = new Drive(args[0], args[1]);
		}
		else if(n == 3)
		{
			inst->m_currentRoutine = new Drive(args[0], args[1], args[2]);
		}
		else if(n == 4)
		{
			if(!lua_isboolean(L, 4)) error = true;
			else
			{
				bool b = lua_toboolean(L, 4);
				inst->m_currentRoutine = new Drive(args[0], args[1], args[2], b);
			}
		}
		else
		{
			error = true;
		}
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_Turn(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	double args[3];
	
	for(int i = 0; i < n && i < 3; i++) // i < 3 because overload with most params is turn(double, double, double, bool)
	{
		if(!lua_isnumber(L, i + 1)) // +1 because lua is 1-based
		{
			error = true;
			break;
		}
		
		args[i] = lua_tonumber(L, i + 1);
	}
	
	if(!error)
	{
		if(n == 1)
		{
			inst->m_currentRoutine = new Turn(args[0]);
		}
		else if(n == 2)
		{
			inst->m_currentRoutine = new Turn(args[0], args[1]);
		}
		else if(n == 3)
		{
			inst->m_currentRoutine = new Turn(args[0], args[1], args[2]);
		}
		else
		{
			error = true;
		}
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_JitterTurn(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	double args[3];
	
	for(int i = 0; i < n && i < 3; i++) // i < 3 because overload with most params is JitterTurn(double, double, double, bool)
	{
		if(!lua_isnumber(L, i + 1)) // +1 because lua is 1-based
		{
			error = true;
			break;
		}
		
		args[i] = lua_tonumber(L, i + 1);
	}
	
	if(!error)
	{
		if(n == 2)
		{
			inst->m_currentRoutine = new JitterTurn(args[0], args[1]);
		}
		else if(n == 3)
		{
			inst->m_currentRoutine = new JitterTurn(args[0], args[1], args[2]);
		}
		else
		{
			error = true;
		}
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_Fire(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	if(n == 0)
	{
		inst->m_currentRoutine = new Fire();
	}
	else if(n == 1)
	{
		if(!lua_isboolean(L, 1)) error = true;
		else inst->m_currentRoutine = new Fire(lua_toboolean(L, 1));
	}
	else
	{
		error = true;
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_DribbleDrive(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	double args[4];
	
	for(int i = 0; i < n && i < 4; i++) // i < 4 because overload with most params is dribbleDrive(double, double, double, double, bool)
	{
		if(!lua_isnumber(L, i + 1)) // +1 because lua is 1-based
		{
			error = true;
			break;
		}
		
		args[i] = lua_tonumber(L, i + 1);
	}
	
	if(!error)
	{
		Automation* drive = NULL;
		double wait = 1.0;
		
		if(n == 1)
		{
			drive = new Drive(args[0]);
			// wait defaults to 1.0 from above
		}
		else if(n == 2)
		{
			drive = new Drive(args[0]);
			wait = args[1];
		}
		else if(n == 3)
		{
			drive = new Drive(args[0], args[2]);
			wait = args[1];
		}
		else if(n == 4)
		{
			drive = new Drive(args[0], args[2], args[3]);
			wait = args[1];
		}
		else if(n == 5)
		{
			if(!lua_isboolean(L, 5)) error = true;
			else
			{
				bool b = lua_toboolean(L, 5);
				
				drive = new Drive(args[0], args[2], args[3], b);
				wait = args[1];
			}
		}
		else
		{
			error = true;
		}
		
		// still no error?
		if(!error)
		{
			Parallel* routine = new Parallel("DribbleDrive", false, false, true);
			Sequential* dribble = new Sequential("DribbleDriveWait");
			dribble->AddAutomation(new Pause(wait));
			dribble->AddAutomation(drive);
			routine->AddAutomation(dribble);
			routine->AddAutomation(new Dribble());
			
			inst->m_currentRoutine = routine;
		}
	}
	
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_LoadLauncher(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	if(n == 0)
	{
		inst->m_currentRoutine = new LoadLauncher();
	}
	else
	{
		error = true;
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int lua_HotLeftStatus(lua_State* L)
{
	bool isActive = SensorFactory::GetHotGoal()->GetActiveSide() == HotGoal::LEFT;
	
	lua_pushboolean(L, isActive);
	
	return 1;
}

int lua_HotRightStatus(lua_State* L)
{
	bool isActive = SensorFactory::GetHotGoal()->GetActiveSide() == HotGoal::RIGHT;

	lua_pushboolean(L, isActive);
	
	return 1;
}

bool LuaScript::Run()
{
	if(m_error) return true;
	
	if(m_currentRoutine == NULL || m_currentRoutine->Update())
	{
		// dispose of the old routine
		delete m_currentRoutine; // can safely delete -- m_currentRoutine is either NULL or a completed routine; delete NULL is a no-op
		m_currentRoutine = NULL;
		
		// run until the next (or first) routine
		
		ExecutionState::Enum state = m_scriptProvider.Step();
		
		if(state == ExecutionState::EXEC_ERROR)
		{
			// ya done goofed now, son
			m_error = true;
			BufferedConsole::Printfln("Error executing lua script: %s", lua_tostring(m_scriptProvider.L(), -1));
			
			return true; // houston, abort!!!
		}
		
		if(state == ExecutionState::PAUSED)
		{
			// script paused -- we probably have a new routine to run
			if(m_currentRoutine == NULL)
			{
				// ...?  don't do anything -- next script segment will be run on next call to Run() because m_currentRoutine == NULL
			}
			else
			{
				// new routine
				bool res = m_currentRoutine->StartAutomation(GetStartEvent());
				
				if(!res) // something went wrong...
				{
					// ignore it and keep going by unsetting it
					delete m_currentRoutine;
					m_currentRoutine = NULL;
					// fall through to return state == ExecutionState::FINISHED (== false)
				}
			}
		}
		
		return state == ExecutionState::FINISHED; // return true if we finished, false if the script is paused
	}
	else
	{
		// we don't need to do anything except return -- if we have a routine, it's already been updated because
		// of the call in the original if statement ("m_currentRoutine->Update")
		
		return false; // we're not done yet -- script isn't done executing
	}
	
	// should never happen
//	BufferedConsole::Printfln("the universe broke");
	return true;
}

bool LuaScript::Abort()
{
	if(m_currentRoutine != NULL)
	{
		m_currentRoutine->AbortAutomation(GetAbortEvent());
	}
	
	return true;
}

void LuaScript::AllocateResources()
{
	if(m_error) return;
	
	for(std::vector<std::string>::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		std::string resource = *it;
		
		if(resource == "DRIVE")	AllocateResource(ControlResource::DRIVE);
		else if(resource == "TURN") AllocateResource(ControlResource::TURN);
		else if(resource == "COLLECTOR_ARM") AllocateResource(ControlResource::COLLECTOR_ARM);
		else if(resource == "COLLECTOR_ROLLERS") AllocateResource(ControlResource::COLLECTOR_ROLLERS);
		else if(resource == "LAUNCHER_ANGLE") AllocateResource(ControlResource::LAUNCHER_ANGLE);
		else if(resource == "LAUNCHER_LOADER") AllocateResource(ControlResource::LAUNCHER_LOADER);
		else if(resource == "PRESSURE_PLATE") AllocateResource(ControlResource::PRESSURE_PLATE);
	}
}
