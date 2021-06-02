#include "hook.h"
#include "../debug.h"
#include <lua.h>
#include <lualib.h>

void error(lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(EXIT_FAILURE);
}

/**
 * Open the file
 *
 * This part is abstracted out so that the lua_State can be passed
 * directly to the encoders
 *
 * */
int lua_hook_load(const char *script_name, lua_State *L) {
  // so that I can print stuff; remove later
  luaL_openlibs(L);

  // stack is currently empty
  int ret = luaL_loadfile(L, script_name);

  switch (ret) {
    case LUA_OK:
    case LUA_ERRFILE:
      // Couldn't read the file: assume no script intended
      break;
    case LUA_ERRRUN:
      error(L, "Failed loading script '%s': runtime error: %s", script_name,
            lua_tostring(L, -1));
      break;
    case LUA_ERRERR:
      error(L, "Failed loading script '%s': message handler error: %s", script_name,
            lua_tostring(L, -1));
      break;
    case LUA_ERRSYNTAX:
      error(L, "Failed loading script '%s': syntax error: %s", script_name,
            lua_tostring(L, -1));
      break;
    case LUA_ERRMEM:
      error(L, "Failed loading script '%s': out-of-memory error: %s", script_name,
            lua_tostring(L, -1));
      break;
    default:
      error(L, "Failed loading script '%s': unknown error: %s", script_name,
            lua_tostring(L, -1));
      break;
	}
  return ret;
}

/**
 * Encoders
 *
 * The difference between these and those in FRR is that these
 * must also do the working of placing themselves on the stack (or in globals)
 *
 * */
void lua_fromInteger(lua_State *L, const char *bindname, int a) {
  lua_pushinteger(L, a);
  lua_setglobal(L, bindname);
}

void lua_fromDouble(lua_State *L, const char *bindname, double a) {
  lua_pushnumber(L, a);
  lua_setglobal(L, bindname);
}

/**
 * Perform the actual call
 *
 * The globals have already been placed, so this part is straightforward
 *
 * */
int lua_hook_call(lua_State *L) {
  int ret = lua_pcall(L, 0, 0, 0);

  switch (ret) {
  case LUA_OK:
    break;
  default:
    error(L, "Script unknown error: %s",
          lua_tostring(L, -1));
    break;
  }

  if (ret != LUA_OK) {
    lua_pop(L, 1);
    goto done;
  }

done:
  /* LUA_OK is 0, so we can just return lua_pcall's result directly */
  return ret;
}