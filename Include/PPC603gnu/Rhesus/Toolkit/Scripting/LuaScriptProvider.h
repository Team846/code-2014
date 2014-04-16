#include "ScriptProvider.h"

#include <string>

#include "LuaExposer.h"

namespace Rhesus 
{
namespace Toolkit 
{
namespace Scripting 
{

class LuaScriptProvider : public ScriptProvider
{
public:
	LuaScriptProvider();
	~LuaScriptProvider();
	
	bool LoadScript(std::string script);
	bool LoadFromFile(std::string file);
	
	void CreateContext();
	void ExposeEntity(LuaExposer exposer);
	void ExposeEntity(std::string name, lua_CFunction func);
	
	ExecutionState::Enum Step();
	
	lua_State* L();
	
private:
	lua_State* _L;
	
	bool _run;
};

}
}
}
