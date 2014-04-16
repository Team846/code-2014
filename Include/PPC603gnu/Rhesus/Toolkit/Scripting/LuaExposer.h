#ifndef RHESUS_LUA_EXPOSER_H_
#define RHESUS_LUA_EXPOSER_H_

#include "RHESUS_internal_lib__/RHESUS_lua__/LuaHeader.h"

namespace Rhesus 
{
namespace Toolkit 
{
namespace Scripting 
{

typedef void (*LuaExposer)(lua_State* L);

}
}
}

#endif
