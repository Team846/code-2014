#include "LuaScript.h"

#include "Collect.h"
#include "Drive.h"
#include "Turn.h"
#include "JitterTurn.h"
#include "Fire.h"
#include "Dribble.h"
#include "LoadLauncher.h"
#include "Parallel.h"
#include "Sequential.h"
#include "Pause.h"
#include "../../Sensors/CheesyVisionServer.h"

#include <Rhesus.Toolkit.IO.h>
#include <Rhesus.Toolkit.Utilities.h>

using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Scripting;
using namespace Rhesus::Toolkit::Utilities;

LuaScript::LuaScript(std::string file)
	: Automation("LuaScript"), m_file(file), m_scriptProvider(NULL), m_currentRoutine(NULL), m_error(false), m_allocateError(false), m_resources(), m_actionGroups(), m_isGrouping(false), m_currentGroup("__default__")
{
}

LuaScript::~LuaScript()
{
	cleanupActionGroups();
	
	delete m_currentRoutine;
	m_currentRoutine = NULL;
	
	delete m_scriptProvider;
	m_scriptProvider = NULL;
}

void LuaScript::cleanupActionGroups()
{
	for(std::hash_map<std::string, std::vector<Automation*> >::iterator it = m_actionGroups.begin(); it != m_actionGroups.end(); ++it)
	{
		std::vector<Automation*>& vec = it->second;
		
		vec.erase(std::remove_if(vec.begin(), vec.end(), ContainerCleanup::DeleteVector<Automation*>), vec.end());
	}
	
	m_actionGroups.clear();
	m_isGrouping = false;
}

bool LuaScript::Start()
{
	if(m_allocateError) return false; // AllocateResource failed
	
	delete m_scriptProvider; // if m_scriptProvider is NULL it's fine because delete NULL is a no-op.
	m_scriptProvider = NULL;
	
	m_scriptProvider = new LuaScriptProvider();
	m_currentRoutine = NULL;
	m_error = false;
	m_resources.clear();
	cleanupActionGroups(); // deallocates action groups and sets isGrouping = false
	
	m_scriptProvider->CreateContext();

	if(!m_scriptProvider->LoadFromFile(m_file))
	{
		BufferedConsole::Printfln("Error loading script file: %s", lua_tostring(m_scriptProvider->L(), -1));
		m_error = true;
		return false;
	}

	lua_State* L =  m_scriptProvider->L();
	
	lua_pushinteger(L, reinterpret_cast<size_t>(this));
	lua_setglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	
	m_scriptProvider->ExposeEntity("collect", (lua_CFunction)lua_Collect);
	m_scriptProvider->ExposeEntity("drive", (lua_CFunction)lua_Drive);
	m_scriptProvider->ExposeEntity("turn", (lua_CFunction)lua_Turn);
	m_scriptProvider->ExposeEntity("jitterTurn", (lua_CFunction)lua_JitterTurn);
	m_scriptProvider->ExposeEntity("fire", (lua_CFunction)lua_Turn);
	m_scriptProvider->ExposeEntity("dribbleDrive", (lua_CFunction)lua_DribbleDrive);
	m_scriptProvider->ExposeEntity("loadLauncher", (lua_CFunction)lua_LoadLauncher);
	m_scriptProvider->ExposeEntity("getHotLeftStatus", (lua_CFunction)lua_HotLeftStatus);
	m_scriptProvider->ExposeEntity("getHotRightStatus", (lua_CFunction)lua_HotRightStatus);
	m_scriptProvider->ExposeEntity("BufferedPrint", (lua_CFunction)lua_BufferedPrint);
	
	m_scriptProvider->ExposeEntity("beginActionGroup", (lua_CFunction)lua_BeginActionGroup);
	m_scriptProvider->ExposeEntity("endActionGroup", (lua_CFunction)lua_EndActionGroup);
			
	return true;
}

void LuaScript::setCurrentOrGroup(Automation* routine)
{
	if(m_isGrouping)
	{
		m_actionGroups[m_currentGroup].push_back(routine);
	}
	else
	{
		m_currentRoutine = routine;
	}
}

int LuaScript::lua_Collect(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	lua_pop(L, 1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	if(n == 0)
	{
		inst->setCurrentOrGroup(new Collect());
	}
	else
	{
		BufferedConsole::Printfln("Collect: error");
		error = true;
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_Drive(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	lua_pop(L, 1);
	
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
			inst->setCurrentOrGroup(new Drive(args[0]));
		}
		else if(n == 2)
		{
			inst->setCurrentOrGroup(new Drive(args[0], args[1]));
		}
		else if(n == 3)
		{
			inst->setCurrentOrGroup(new Drive(args[0], args[1], args[2]));
		}
		else if(n == 4)
		{
			if(!lua_isboolean(L, 4)) error = true;
			else
			{
				bool b = lua_toboolean(L, 4);
				inst->setCurrentOrGroup(new Drive(args[0], args[1], args[2], b));
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
	
	lua_pop(L, 1);
	
	int n = lua_gettop(L);
	
	BufferedConsole::Printfln("Turn n: %d", n);
	
	bool error = false;
	
	double args[3];
	
	for(int i = 0; i < n && i < 3; i++) // i < 3 because overload with most params is turn(double, double, double, bool)
	{
		if(!lua_isnumber(L, i + 1)) // +1 because lua is 1-based
		{
			BufferedConsole::Printfln("Turn not number index %d", i);
			error = true;
			break;
		}
		
		args[i] = lua_tonumber(L, i + 1);
	}
	
	if(!error)
	{
		if(n == 1)
		{
			inst->setCurrentOrGroup(new Turn(args[0]));
		}
		else if(n == 2)
		{
			inst->setCurrentOrGroup(new Turn(args[0], args[1]));
		}
		else if(n == 3)
		{
			inst->setCurrentOrGroup(new Turn(args[0], args[1], args[2]));
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
	
	lua_pop(L, 1);
	
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
			inst->setCurrentOrGroup(new JitterTurn(args[0], args[1]));
		}
		else if(n == 3)
		{
			inst->setCurrentOrGroup(new JitterTurn(args[0], args[1], args[2]));
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
	
	lua_pop(L, 1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	if(n == 0)
	{
		inst->setCurrentOrGroup(new Fire());
	}
	else if(n == 1)
	{
		if(!lua_isboolean(L, 1)) error = true;
		else inst->setCurrentOrGroup(new Fire(lua_toboolean(L, 1)));
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
	
	lua_pop(L, 1);
	
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
			
			inst->setCurrentOrGroup(routine);
		}
	}
	
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_LoadLauncher(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	lua_pop(L, 1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	if(n == 0)
	{
		inst->setCurrentOrGroup(new LoadLauncher());
		BufferedConsole::Printfln("LoadLauncher");
	}
	else
	{
		error = true;
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_RunParallel(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	lua_pop(L, 1);
	
	int n = lua_gettop(L);
	bool error = false;
	
	std::vector<Automation*> sequentials;
	
	for(int i = 0; i < n; i++)
	{
		if(!lua_isstring(L, i+1)) // i+1 because lua is one-based
		{
			BufferedConsole::Printfln("LuaScript: WARNING; invalid parameter passed to runParallel; notAString.");
			error = true;
			break;
		}
		
		const char* actionGroupNameC = lua_tostring(L, i+1);
		std::string actionGroupName = std::string(actionGroupNameC); // convert to an std string
		
		if(inst->m_actionGroups.find(actionGroupName) == inst->m_actionGroups.end())
		{
			BufferedConsole::Printfln("LuaScript: WARNING; invalid parameter passed to runParallel; actionGroupNameNotInMap.");
			error = true;
			break;
		}
		
		std::vector<Automation*>& actionGroup = inst->m_actionGroups[actionGroupName];
		
		sequentials.push_back(new Sequential(actionGroupName, actionGroup));
	}
	
	if(!error)
	{
		Parallel* p = new Parallel("lua::runParallel", sequentials);
		
		inst->setCurrentOrGroup(p);
	}
	
	return error ? 0 
			: lua_yield(L, 0); // lua_yield pauses execution
}

int LuaScript::lua_HotLeftStatus(lua_State* L)
{
	bool isActive = CheesyVisionServer::GetInstance()->GetLeftStatus();
	
	lua_pushboolean(L, isActive);
	
	return 1;
}

int LuaScript::lua_HotRightStatus(lua_State* L)
{
	bool isActive = CheesyVisionServer::GetInstance()->GetRightStatus();

	lua_pushboolean(L, isActive);
	
	return 1;
}

int LuaScript::lua_BufferedPrint(lua_State* L)
{
	int n = lua_gettop(L);
	
	if(n == 1 && lua_isstring(L, 1))
	{
		const char* str = lua_tostring(L, 1);
		BufferedConsole::Printfln(str);
	}
	
	return 0;
}

int LuaScript::lua_BeginActionGroup(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	lua_pop(L, 1);
	
	int n = lua_gettop(L);
	
	inst->m_isGrouping = true; // enable grouping
	
	if(n == 1 && lua_isstring(L, 1))
	{
		const char* str = lua_tostring(L, 1);
		
		inst->m_currentGroup = std::string(str); // set the current group to be the passed arg
	}
	// otherwise use the last m_currentGroup
	
	return 0;
}

int LuaScript::lua_EndActionGroup(lua_State* L)
{
	lua_getglobal(L, "__LRT_LUASCRIPT_INSTANCE");
	LuaScript* inst = (LuaScript*)(size_t)lua_tointeger(L, -1);
	
	lua_pop(L, 1);
	
	if(!inst->m_isGrouping)
	{
		BufferedConsole::Printfln("LuaScript: WARNING; mismatched extra endActionGroup() called."); // warn if this isn't paired with a begin
	}
	
	inst->m_isGrouping = false;
	
	return 0;
}

bool LuaScript::Run()
{
	if(m_error) return true;
	
	if(m_currentRoutine == NULL || m_currentRoutine->Update())
	{
		BufferedConsole::Printfln("Next step");
		
		// dispose of the old routine
		delete m_currentRoutine; // can safely delete -- m_currentRoutine is either NULL or a completed routine; delete NULL is a no-op
		m_currentRoutine = NULL;
		
		// run until the next (or first) routine
		
		ExecutionState::Enum state = m_scriptProvider->Step();
		
		if(state == ExecutionState::EXEC_ERROR)
		{
			// ya done goofed now, son
			m_error = true;
			BufferedConsole::Printfln("Error executing lua script: %s", lua_tostring(m_scriptProvider->L(), -1));
			
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
	
	BufferedConsole::Printfln("Aborting");
	
	return true;
}

void LuaScript::AllocateResources()
{
	m_allocateError = false;
	
	LuaScriptProvider manifest;
	manifest.CreateContext();
	manifest.LoadFromFile(m_file + std::string(".manifest")); // load manifest
	manifest.Step(); // run once to load functions
	
	lua_State* L = manifest.L();
	
	lua_getglobal(L, "AllocateResources");
	
	if(lua_pcall(L, 0, 1, 0) != 0)
	{
		BufferedConsole::Printfln("Error calling the function: %s", lua_tostring(L, -1));
		m_allocateError = true;
		return;
	}
	
	if(lua_gettop(L) != 1 || !lua_istable(L, 1))
	{
		BufferedConsole::Printfln("Invalid return value.");
		
		if(lua_gettop(L) != 1) BufferedConsole::Printfln("top != 1");
		if(!lua_istable(L, 1)) BufferedConsole::Printfln("Not a table");
		
		m_allocateError = true;
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
	
	for(std::vector<std::string>::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		std::string resource = *it;
		
		if(resource == "DRIVE")	AllocateResource(ControlResource::DRIVE);
		else if(resource == "TURN") AllocateResource(ControlResource::TURN);
		else if(resource == "COLLECTOR_ARM") AllocateResource(ControlResource::COLLECTOR_ARM);
		else if(resource == "COLLECTOR_ROLLERS") AllocateResource(ControlResource::COLLECTOR_ROLLERS);
		else if(resource == "LAUNCHER_ANGLE") AllocateResource(ControlResource::LAUNCHER_ANGLE);
		else if(resource == "LAUNCHER_LOADER") AllocateResource(ControlResource::LAUNCHER_LOADER);
		else if(resource == "BALL_HOLDER") AllocateResource(ControlResource::BALL_HOLDER);
	}
}
