#include "LuaScriptProvider.h"

#include "RHESUS_internal_lib__/RHESUS_lua__/LuaHeader.h"

using namespace Rhesus::Toolkit::Scripting;

LuaScriptProvider::LuaScriptProvider()
{
	_L = NULL;
	_run = false;
}

LuaScriptProvider::~LuaScriptProvider()
{
	lua_close(_L);
}

bool LuaScriptProvider::LoadScript(std::string script)
{
	if(_L == NULL || _run) return false;
	
	return luaL_loadstring(_L, script.c_str()) == 0;
}

bool LuaScriptProvider::LoadFromFile(std::string file)
{
	if(_L == NULL || _run) return false;
	
	return luaL_loadfile(_L, file.c_str()) == 0;
}

void LuaScriptProvider::CreateContext()
{
	if(_L != NULL || _run) return;
	
	_L = luaL_newstate();
	
	luaL_openlibs(_L);
}

void LuaScriptProvider::ExposeEntity(LuaExposer exposer)
{
	if(_L != NULL || _run) return;
	
	exposer(_L);
}

void LuaScriptProvider::ExposeEntity(std::string name, lua_CFunction func)
{
	if(_L == NULL || _run || func == NULL) return;
	
	lua_register(_L, name.c_str(), func);
}

ExecutionState::Enum LuaScriptProvider::Step()
{
	if(_L == NULL) return ExecutionState::EXEC_ERROR;
	
	_run = true;
	
	int result = lua_resume(_L, NULL, 0);
	
	ExecutionState::Enum res = ExecutionState::EXEC_ERROR;
	
	switch(result)
	{
	case 0:
		res = ExecutionState::FINISHED;
		break;
	case LUA_YIELD:
		res = ExecutionState::PAUSED;
		break;
	}
	
	return res;
}

// this method is dangerous and is for the sole purpose of being able
// to call Lua methods...in the future add a lua method call method
lua_State* LuaScriptProvider::L()
{
	return _L;
}
