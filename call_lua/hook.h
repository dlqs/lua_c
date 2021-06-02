#ifndef HOOK_H
#define HOOK_H

#include "lualib.h"
#include <lauxlib.h>
#include <lua.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// From compiler.h
#define _MACRO_VARIANT(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, N, ...) N

#define _CONCAT2(a, b) a##b
#define _CONCAT(a, b) _CONCAT2(a, b)

#define MACRO_VARIANT(NAME, ...)                                               \
  _CONCAT(NAME, _MACRO_VARIANT(0, ##__VA_ARGS__, _10, _9, _8, _7, _6, _5, _4,  \
                               _3, _2, _1, _0))

#define NAMECTR(name) _CONCAT(name, __COUNTER__)

/* per-arg repeat macros, use like:
 * #define PERARG(n) ...n...
 * #define FOO(...) MACRO_REPEAT(PERARG, ##__VA_ARGS__)
 */

#define _MACRO_REPEAT_0(NAME)
#define _MACRO_REPEAT_1(NAME, A1) NAME(A1)
#define _MACRO_REPEAT_2(NAME, A1, A2) NAME(A1) NAME(A2)
#define _MACRO_REPEAT_3(NAME, A1, A2, A3) NAME(A1) NAME(A2) NAME(A3)
#define _MACRO_REPEAT_4(NAME, A1, A2, A3, A4)                                  \
  NAME(A1) NAME(A2) NAME(A3) NAME(A4)
#define _MACRO_REPEAT_5(NAME, A1, A2, A3, A4, A5)                              \
  NAME(A1) NAME(A2) NAME(A3) NAME(A4) NAME(A5)
#define _MACRO_REPEAT_6(NAME, A1, A2, A3, A4, A5, A6)                          \
  NAME(A1) NAME(A2) NAME(A3) NAME(A4) NAME(A5) NAME(A6)
#define _MACRO_REPEAT_7(NAME, A1, A2, A3, A4, A5, A6, A7)                      \
  NAME(A1) NAME(A2) NAME(A3) NAME(A4) NAME(A5) NAME(A6) NAME(A7)
#define _MACRO_REPEAT_8(NAME, A1, A2, A3, A4, A5, A6, A7, A8)                  \
  NAME(A1) NAME(A2) NAME(A3) NAME(A4) NAME(A5) NAME(A6) NAME(A7) NAME(A8)

#define MACRO_REPEAT(NAME, ...)                                                \
  MACRO_VARIANT(_MACRO_REPEAT, ##__VA_ARGS__)(NAME, ##__VA_ARGS__)
// End

// Hook functions
extern int lua_hook_load(const char *script_name, lua_State *L);

extern int lua_hook_call(lua_State *L);

// Encoding functions

extern void lua_fromInteger(lua_State *L, const char *bindname, int a);

extern void lua_fromDouble(lua_State *L, const char *bindname, double a);

#define ENCODE_ARGS_WITH_STATE(x) ENCODE_ARGS(L, x)

#define ENCODE_ARGS(L, x) _Generic((x),     \
    int    : lua_fromInteger(L, #x, x),      \
    double : lua_fromDouble(L, #x, x)       \
  );

#define lua_hook(script_name, ...)                        \
  do {                                                    \
     lua_State *L = luaL_newstate();                      \
     if (lua_hook_load(script_name, L) == 0) {            \
       MACRO_REPEAT(ENCODE_ARGS_WITH_STATE, ##__VA_ARGS__) \
       lua_hook_call(L);                                  \
     }                                                    \
     lua_close(L);                                        \
  } while (0)

#endif // HOOK_H
