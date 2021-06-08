#ifndef HOOK_H
#define HOOK_H

#include "lualib.h"
#include <lauxlib.h>
#include <lua.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libpp/map-lists.h"
#include "libpp/separators.h"

// Took the variadic macros from compiler.h
// https://github.com/frrouting/frr/blob/9dddf5fe699518f28e028541c9a57e8fde2402c4/lib/compiler.h#L143-L183
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

// Encoder/decoder functions
void lua_fromInteger(lua_State *L, const char *bindname, int a);

void lua_toInteger(lua_State *L, const char *bindname, int a);

void lua_fromIntegerPtr(lua_State *L, const char *bindname, int *a);

void lua_toIntegerPtr(lua_State *L, const char *bindname, int* a);

void lua_do_nothing(lua_State *L, const char *bindname);

// User-defined struct
typedef struct person {
  char *name;
  int age;
} person;

// Encoder/decoder functions for user-defined struct
void lua_fromPersonPtr(lua_State *L, const char *bindname, person *p);

void lua_toPersonPtr(lua_State *L, const char *bindname, person *p);

/**
 * Map types to their respective encoder functions here.
 * We use _Generic to dispatch based on type.
 * The encoder functions bind the variable name to their values
 *
 * Do not put a default case; we *want* a compile error if an encoder function cannot be found.
 *
 **/
#define ENCODE_ARGS_WITH_STATE(L, name, value) _Generic((value),     \
    int     : lua_fromInteger,                                       \
    int *   : lua_fromIntegerPtr,                                    \
    person *: lua_fromPersonPtr                                      \
  )(L, name, value)

#define DECODE_ARGS_WITH_STATE(L, name, value) _Generic((value),     \
    int     : lua_toInteger,                                         \
    int *   : lua_toIntegerPtr,                                      \
    person *: lua_toPersonPtr                                        \
  )(L, name, value)

// Insert lua state (L) as first argument
#define ENCODE_ARGS(name, value) ENCODE_ARGS_WITH_STATE(L, name, value)

#define DECODE_ARGS(name, value) DECODE_ARGS_WITH_STATE(L, name, value)

/**
 * This macro takes a script name and variables whose types have encoder functions mapped above
 *
 * The variables should be lvalues, not rvalues, or else we won't have a name to bind them to
 *
 * This macro has 3 parts:
 * 1. Loading the script into Lua state
 * 2. Encoding the arguments into Lua state (this is done by mapping ENCODE_ARGS over the var args)
 * 3. Calling the Lua state
 * */
#define lua_hook(script_name, ...)                                    \
  do {                                                                \
     lua_State *L = luaL_newstate();                                  \
     if (lua_hook_load(script_name, L) == 0) {                        \
       PP_MAP_LISTS( ENCODE_ARGS, PP_SEP_SEMICOLON, ##__VA_ARGS__);   \
       if (lua_hook_call(L) == 0) {                                   \
         PP_MAP_LISTS( DECODE_ARGS, PP_SEP_SEMICOLON, ##__VA_ARGS__); \
       }                                                              \
     }                                                                \
     lua_close(L);                                                    \
  } while (0)

#endif // HOOK_H
