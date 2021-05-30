#ifndef ARRAY_H
#define ARRAY_H

#include <lua.h>
#include <lauxlib.h>
#include <limits.h>
#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD))
#define checkarray(L)                           \
  (BitArray *)luaL_checkudata(L, 1, "LuaBook.array")

int luaopen_array(lua_State *L);

#endif // ARRAY_H
