#include "LuaHeader.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <Windows.h>

class Routines
{
public:
	static int Drive(lua_State* L)
	{
		std::printf("Drive!\n");

		return lua_yield(L, 0);
	}

	static int Shoot(lua_State* L)
	{
		std::printf("Shoot!\n");

		return lua_yield(L, 0);
	}
};

lua_State* L;
time_t lastStart = -1;

bool startRoutine()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "drive", (lua_CFunction)Routines::Drive);
	lua_register(L, "shoot", (lua_CFunction)Routines::Shoot);

	int status = luaL_loadfile(L, "Autonomous.lua");

	return !status || status == LUA_YIELD;
}

int main()
{
	std::printf("--RobotInit--\n");

	startRoutine();

	std::printf("-------------\n");

	int tick = 0;

	while(true)
	{
		if(tick++ % 100 == 0)
			std::printf("Main loop: %d\n", tick - 1);

		if(lastStart > 0 && std::time(NULL) - lastStart < 5)
		{
			Sleep(20);
			continue; // wait until 1 second has elapsed.
		}

		lastStart = std::time(NULL);
		int result = lua_resume(L, NULL, 0);

		if(result == 0)
		{
			std::printf("!!! Routine complete. !!!\n");
			break;
		}
		else if(result == LUA_YIELD)
		{

		}
		else
		{
			std::printf("An error occurred. Error: %s\n", lua_tostring(L, 1));
			break;
		}

		Sleep(20);
	}

	lua_close(L);

	system("PAUSE");

	return 0;
}