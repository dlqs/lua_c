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
#include <assert.h>

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
/* Some insane macros to count number of varargs to a functionlike macro */
#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                                        \
         62, 61, 60,                                       \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_NARG(...)     PP_NARG_(_, ##__VA_ARGS__, PP_RSEQ_N())


#define ESC(...) __VA_ARGS__
#define MAP_LISTS(M, ...)                                                      \
	_CONCAT(_MAP_LISTS_, PP_NARG(__VA_ARGS__))(M, ##__VA_ARGS__)
#define _MAP_LISTS_0(M)
#define _MAP_LISTS_1(M, _1) ESC(M _1)
#define _MAP_LISTS_2(M, _1, _2) ESC(M _1; M _2)
#define _MAP_LISTS_3(M, _1, _2, _3) ESC(M _1; M _2; M _3)
#define _MAP_LISTS_4(M, _1, _2, _3, _4) ESC(M _1; M _2; M _3; M _4)
#define _MAP_LISTS_5(M, _1, _2, _3, _4, _5) ESC(M _1; M _2; M _3; M _4; M _5)
#define _MAP_LISTS_6(M, _1, _2, _3, _4, _5, _6)                                \
	ESC(M _1; M _2; M _3; M _4; M _5; M _6)
#define _MAP_LISTS_7(M, _1, _2, _3, _4, _5, _6, _7)                            \
	ESC(M _1; M _2; M _3; M _4; M _5; M _6; M _7)
#define _MAP_LISTS_8(M, _1, _2, _3, _4, _5, _6, _7, _8)                        \
	ESC(M _1; M _2; M _3; M _4; M _5; M _6; M _7; M _8)


void lua_pushintegerp(lua_State *L, int *num);

void lua_decode_integerp(lua_State *L, int idx, int *num);
  
void lua_decode_noop(lua_State *L, int idx, int n);

#define ENCODE_ARGS(name, value)                                               \
	do {                                                                   \
		ENCODE_ARGS_WITH_STATE(L, value);                              \
	} while (0)

#define DECODE_ARGS(name, value)                                               \
	do {                                                                   \
      printf("lua decode top: %d\n", lua_gettop(L)); \
	  assert(lua_gettop(L) == 1);  \
		lua_getfield(L, 1, name);    \
		DECODE_ARGS_WITH_STATE(L, value);                              \
	  assert(lua_gettop(L) == 1);  \
	} while (0)

#define ENCODE_ARGS_WITH_STATE(L, value)                                       \
	_Generic((value), \
int * : lua_pushintegerp                                 \
)(L, value)

#define DECODE_ARGS_WITH_STATE(L, v)                                       \
	_Generic((v),                       \
int * : lua_decode_integerp                              \
)(L, -1, v)

/*
 * Call script.
 *
 * fs
 *    The script to call; this is obtained from frrscript_load().
 *
 * Returns:
 *    0 if the script ran successfully, nonzero otherwise.
 */
int _frrscript_call(lua_State *L, int n);

/*
 * Wrapper for call script. Maps values passed in to their encoder
 * and decoder types.
 *
 * fs
 *    The script to call; this is obtained from frrscript_load().
 *
 * Returns:
 *    0 if the script ran successfully, nonzero otherwise.
 */
#define frrscript_call(f, ...)                                              \
	({																		\
		lua_State *L = luaL_newstate();										\
        luaL_dofile(L, "script1.lua");  \
		lua_getglobal(L, f);           \
		assert(lua_isfunction(L, lua_gettop(L)));   \
		MAP_LISTS(ENCODE_ARGS, ##__VA_ARGS__);                         \
		int ret = _frrscript_call(L, PP_NARG(__VA_ARGS__));                              \
		if (ret == 0) {                                                \
			MAP_LISTS(DECODE_ARGS, ##__VA_ARGS__);                 \
		}                                                              \
		ret;                                                           \
	})



#endif // HOOK_H
