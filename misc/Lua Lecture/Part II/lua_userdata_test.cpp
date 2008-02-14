#include <iostream>

extern "C"
{
  #include "lua/lua.h"
  #include "lua/lualib.h"
  #include "lua/lauxlib.h"
}

class myMath
{
  public:
    myMath() : data(0.0) {}
    void add(double a)
    {
      data += a;
    }

    void sub(double a)
    {
      data -= a;
    }

    double get()
    {
      return data;
    }

  private:
    double data;
};

int New(lua_State *Lstate)
{
  // check the parameters passed from left to right, then pull them off in
  // that order from the stack
  if(true)
  {
    using std::cout;
    using std::endl;

    cout << "(C++): new myMath" << endl;

    myMath *m = new myMath();
    lua_pushlightuserdata(Lstate, (void *) m);
  }
  else
  {
    // throw some kind of error, the types don't match up
    // (possibly a corrupt Lua stack)
  }

  // 1 result
  return 1;
}

int Delete(lua_State *Lstate)
{
  // check the parameters passed from left to right, then pull them off in
  // that order from the stack
  if(lua_isuserdata(Lstate, 1))
  {
    void *p = lua_touserdata(Lstate, 1);
    myMath *m = static_cast<myMath *>(p);

    using std::cout;
    using std::endl;

    cout << "(C++): delete myMath" << endl;
    
    delete m;

    // clean up the stack    
    lua_pop(Lstate, 1);
  }
  else
  {
    // throw some kind of error, the types don't match up
    // (possibly a corrupt Lua stack)
  }

  // 0 result
  return 0;
}

int add(lua_State *Lstate)
{
  // check the parameters passed from left to right, then pull them off in
  // that order from the stack
  if(lua_isuserdata(Lstate, 1) &&
     lua_isnumber(Lstate, 2))
  {
    void *p = lua_touserdata(Lstate, 1);
    myMath *m = static_cast<myMath *>(p);
    double a = lua_tonumber(Lstate, 2);

    using std::cout;
    using std::endl;
    
    m->add(a);

    cout << "(C++): myMath->add called with (" << a << ")" << endl;
    
    // clean up the stack    
    lua_pop(Lstate, 2);

    lua_pushnumber(Lstate, (lua_Number) (m->get()));
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
  if(lua_isuserdata(Lstate, 1) &&
     lua_isnumber(Lstate, 2))
  {
    void *p = lua_touserdata(Lstate, 1);
    myMath *m = static_cast<myMath *>(p);
    double a = lua_tonumber(Lstate, 2);

    using std::cout;
    using std::endl;
    
    m->sub(a);

    cout << "(C++): myMath->sub called with (" << a << ")" << endl;
    
    // clean up the stack    
    lua_pop(Lstate, 2);

    lua_pushnumber(Lstate, (lua_Number) (m->get()));
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
  lua_pushstring(Lstate, "New");
  lua_pushcclosure(Lstate, New, 0);
  lua_settable(Lstate, -3);
  lua_pushstring(Lstate, "Delete");
  lua_pushcclosure(Lstate, Delete, 0);
  lua_settable(Lstate, -3);
  
  // pop the table pointer off the stack
  lua_pop(Lstate, 1);

  // execute the global scope portion of the file (you can use this to load
  // files if your code is all inside functions)
  luaL_dofile(Lstate, "lua_userdata_test.lua");

  // close the state (have the interpreter clean up memory, etc)
  lua_close(Lstate);
  return 0;
}
