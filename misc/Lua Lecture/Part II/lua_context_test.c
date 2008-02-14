#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

int main()
{
	/* open the lua state */
	lua_State *Lstate = lua_open();

	/* open the standard libs and functions (needed for print) */
	luaL_openlibs(Lstate);

	/*
	* execute the global scope portion of the file (you can use this to load
	* files if your code is all inside functions)
	*/
	luaL_dofile(Lstate, "lua_context_test.lua");

	/* close the state (have the interpreter clean up memory, etc) */
	lua_close(Lstate);
	return 0;
}
