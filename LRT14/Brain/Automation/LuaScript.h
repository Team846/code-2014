#ifndef LUA_SCRIPT_H_
#define LUA_SCRIPT_H_

#include "Automation.h"

#include <string>
#include <vector>

#include <Rhesus.Toolkit.Scripting.h>

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
	static int lua_HotLeftStatus(lua_State* L);
	static int lua_HotRightStatus(lua_State* L);
	static int lua_BufferedPrint(lua_State* L);
	
	std::string m_file;
	
	Rhesus::Toolkit::Scripting::LuaScriptProvider* m_scriptProvider;
	
	Automation* m_currentRoutine;
	
	bool m_error;
	std::vector<std::string> m_resources;
};

#endif
