#ifndef LUA_SCRIPT_H_
#define LUA_SCRIPT_H_

#include "Automation.h"

#include <string>
#include <vector>

#include <hash_map>

#include <Rhesus.Toolkit.Scripting.h>
#include <Rhesus.Toolkit.Diagnostics.h>

class LuaScript : public Automation
{
public:
	LuaScript(std::string file);
	~LuaScript();
	
	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
private:
	static int lua_Collect(lua_State* L);
	static int lua_Drive(lua_State* L);
	static int lua_Turn(lua_State* L);
	static int lua_JitterTurn(lua_State* L);
	static int lua_Fire(lua_State* L);
	static int lua_DribbleDrive(lua_State* L);
	static int lua_LoadLauncher(lua_State* L);
	static int lua_RunParallel(lua_State* L);
	static int lua_HotLeftStatus(lua_State* L);
	static int lua_HotRightStatus(lua_State* L);
	static int lua_BufferedPrint(lua_State* L);
	static int lua_Pause(lua_State* L);
	static int lua_CollectorMove(lua_State* L);
	static int lua_GetTimeMillis(lua_State* L);
	static int lua_GetGameState(lua_State* L);
	
	static int lua_BeginActionGroup(lua_State* L);
	static int lua_EndActionGroup(lua_State* L);
	
	void cleanupActionGroups();
	
	void setCurrentOrGroup(Automation* routine);
	
	std::string m_file;
	
	Rhesus::Toolkit::Scripting::LuaScriptProvider* m_scriptProvider;
	Rhesus::Toolkit::Diagnostics::Stopwatch m_elapsedSw;
	
	Automation* m_currentRoutine;
	bool m_isParallelizing;
	
	bool m_error;
	bool m_allocateError; // separate so we can reload previously-errored files
	std::vector<std::string> m_resources;
	
	std::hash_map<std::string, std::vector<Automation*> > m_actionGroups;
	bool m_isGrouping;
	std::string m_currentGroup;
};

#endif
