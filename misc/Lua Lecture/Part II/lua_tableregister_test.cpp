#include <iostream>

extern "C"
{
  #include "lua/lua.h"
  #include "lua/lualib.h"
  #include "lua/lauxlib.h"
}

int add(lua_State *Lstate)
{
  // check the parameters passed from left to right, then pull them off in
  // that order from the stack
  if(lua_isnumber(Lstate, 1) &&
     lua_isnumber(Lstate, 2))
  {
    double a = lua_tonumber(Lstate, 1);
    double b = lua_tonumber(Lstate, 2);
    
    using std::cout;
    using std::endl;
    
    cout << "(C++): add called with (" << a << ", " << b << ")" << endl;
    
    // clean up the stack    
    lua_pop(Lstate, 2);

    lua_pushnumber(Lstate, (lua_Number) (a + b));
  }
  else
  {
    // throw some kind of error, the types don't match up
    // (possibly a corrupt Lua stack)
  }

  // 1 result
  return 1;
}

int sub(lua_State *Lstate)
{
  // check the parameters passed from left to right, then pull them off in
  // that order from the stack
  if(lua_isnumber(Lstate, 1) &&
     lua_isnumber(Lstate, 2))
  {
    double a = lua_tonumber(Lstate, 1);
    double b = lua_tonumber(Lstate, 2);
    
    using std::cout;
    using std::endl;

    cout << "(C++): sub called with (" << a << ", " << b << ")" << endl;

    // clean up the stack    
    lua_pop(Lstate, 2);

    lua_pushnumber(Lstate, (lua_Number) (a - b));
  }
  else
  {
    // throw some kind of error, the types don't match up
    // (possibly a corrupt Lua stack)
  }

  // 1 result
  return 1;
}

int main()
{
  // open the lua state
  lua_State *Lstate = lua_open();

  // open the standard libs and functions (needed for print)
  luaL_openlibs(Lstate);
  
  // create a table
  lua_newtable(Lstate);
  // keep a pointer to the table at the top of the stack
  lua_pushvalue(Lstate, -1);
  // set the table as a global
  lua_setfield(Lstate, LUA_GLOBALSINDEX, "myMath");
  
  // register C functions inside the table
  
  // push the name of the function
  lua_pushstring(Lstate, "add");
  // push the C pointer to the function
  lua_pushcclosure(Lstate, add, 0);
  // bind it to the table that was at the top of the stack
  lua_settable(Lstate, -3);
  
  // repeat for the other functions
  lua_pushstring(Lstate, "sub");
  lua_pushcclosure(Lstate, sub, 0);
  lua_settable(Lstate, -3);
  
  // pop the table pointer off the stack
  lua_pop(Lstate, 1);

  // execute the global scope portion of the file (you can use this to load
  // files if your code is all inside functions)
  luaL_dofile(Lstate, "lua_tableregister_test.lua");

  // close the state (have the interpreter clean up memory, etc)
  lua_close(Lstate);
  return 0;
}