#ifndef ARRAY_H
#define ARRAY_H

#include <lua.h>
#include <lauxlib.h>
#include <limits.h>
#include <string.h>
#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD))
#define checkarray(L, i)                           \
  (BitArray *)luaL_checkudata(L, i, "LuaBook.array")

int luaopen_array(lua_State *L);

#endif // ARRAY_H
