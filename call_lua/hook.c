#include <string.h>
#include <lua.h>
#include <lualib.h>
#include "hook.h"
#include "../debug.h"

void error(lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(EXIT_FAILURE);
}

void lua_pushintegerp(lua_State *L, int *num)
{
	lua_pushinteger(L, *num);
}

void lua_decode_integerp(lua_State *L, int idx, int *num)
{
	int isnum;
	*num = lua_tonumberx(L, idx, &isnum);
	lua_pop(L, 1);
	assert(isnum);
}

void lua_decode_noop(lua_State *L, int idx, int num) {}

//void lua_decode_integerp(lua_State *L, int idx, void v) {}

int _frrscript_call(lua_State *L, int nargs)
{

	int ret = lua_pcall(L, nargs, 1, 0);

	switch (ret) {
	case LUA_OK:
		break;
	default:
		printf("Something terrible happened\n");
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

