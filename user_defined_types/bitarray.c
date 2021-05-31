#include "bitarray.h"

typedef struct BitArray {
  int size;
  unsigned int values[1];
} BitArray;

static int newarray(lua_State *L) {
  int i;
  size_t nbytes;
  BitArray *a;

  int n = (int)luaL_checkinteger(L, 1);
  luaL_argcheck(L, n >= 1, 1, "invalid size");
  nbytes = sizeof(BitArray) + I_WORD(n - 1)*sizeof(unsigned int);
  a = (BitArray *)lua_newuserdata(L, nbytes);

  a->size = n;
  for (i = 0; i <= I_WORD(n - 1); i++) {
    a->values[i] = 0;
  }

  luaL_getmetatable(L, "LuaBook.array");
  lua_setmetatable(L, -2);

  return 1;
}

static unsigned int *getparams(lua_State *L, unsigned int *mask) {
  BitArray *a = checkarray(L);
  int index = (int)luaL_checkinteger(L, 2) - 1;
  luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range"); // index
  *mask = I_BIT(index);
  return &a->values[I_WORD(index)];
}

static int setarray(lua_State *L) {
  unsigned int mask;
  unsigned int *entry = getparams(L, &mask);
  luaL_checkany(L, 3);
  if (lua_toboolean(L, 3)) {
    *entry |= mask;
  } else {
    *entry &= ~mask;
  }
  return 0;
}

static int getarray(lua_State *L) {
  unsigned int mask;
  unsigned int *entry = getparams(L, &mask);
  lua_pushboolean(L, *entry & mask);
  return 1;
}

// Library code
static int getsize(lua_State *L) {
  BitArray *a = checkarray(L);
  lua_pushinteger(L, a-> size);
  return 1;
}

int array2string(lua_State *L) {
  BitArray *a = checkarray(L);

  char buf[256];
  for (int i = 0; i < a->size && i < 256; i++) {
    buf[i] = a->values[i] == 1 ? '1' : '0';
  }
  lua_pushfstring(L, "array(%d):%s", a->size, buf);
  return 1;
}

static const struct luaL_Reg arraylib_f [] = {
{"new", newarray},
{NULL, NULL}
};

static const struct luaL_Reg arraylib_m [] = {
{"__newindex", setarray},
{"__index", getarray},
{"__len", getsize},
{"__tostring", array2string},
{NULL, NULL}
};

extern int luaopen_bitarray(lua_State *L) {
  luaL_newmetatable(L, "LuaBook.array");
  luaL_setfuncs(L, arraylib_m, 0);
  luaL_newlib(L, arraylib_f);
  return 1;
}
