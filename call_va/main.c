#include <lauxlib.h>
#include <lua.h>
#include "lualib.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void error(lua_State *L, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

void call_va(lua_State *L, const char *func, const char *sig, ...) {
  va_list vl;
  int narg, nres;
  va_start(vl, sig);
  lua_getglobal(L, func);

  for (narg = 0; *sig; narg++) {
    luaL_checkstack(L, 1, "too many arguments");
    switch (*sig++) {
    case 'd':
      lua_pushnumber(L, va_arg(vl, double));
      break;
    case 'i':
      lua_pushinteger(L, va_arg(vl, int));
      break;
    case 'b':
      lua_pushboolean(L, va_arg(vl, int));
      break;
    case 's':
      lua_pushstring(L, va_arg(vl, char *));
      break;
    case '>':
      goto endargs;
    default:
      error(L, "invalid option (%c)", *(sig - 1));
    }
  }
endargs:

  nres = strlen(sig);

  if (lua_pcall(L, narg, nres, 0) != 0) {
    error(L, "error calling '%s': %s", func, lua_tostring(L, -1));
  }

  nres = -nres;
  while (*sig) {
    switch (*sig++) {
    case 'd': {
      int isnum;
      double n = lua_tonumberx(L, nres, &isnum);
      if (!isnum) {
        error(L, "wrong result type");
      }
      *va_arg(vl, double *) = n;
      break;
    }
    case 'i': {
      int isnum;
      double n = lua_tointegerx(L, nres, &isnum);
      if (!isnum) {
        error(L, "wrong result type");
      }
      *va_arg(vl, int *) = n;
      break;
    }
    case 'b': {
      int n = lua_toboolean(L, nres);
      *va_arg(vl, int *) = n;
      break;
    }
    case 's': {
      const char *s = lua_tostring(L, nres);
      if (s == NULL) {
        error(L, "wrong result type");
      }
      *va_arg(vl, const char **) = s;
      break;
    }
    default:
      error(L, "invalid option (%c)", *(sig - 1));
    }
    nres++;
  }
  va_end(vl);
}

int main() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_dofile(L, "script.lua")) {
      error(L, "Unable to load file");
  }

  // basic data types
  int z;
  call_va(L, "add", "ii>i", 3, 4, &z);
  printf("add returns: %d\n", z);

  double d;
  call_va(L, "div", "ii>d", 1, 3, &d);
  printf("div returns: %.3f\n", d);

  char *s;
  call_va(L, "say_hello", "s>s", "world", &s);
  printf("say_hello returns: %s\n", s);

  int x;
  call_va(L, "xor", "bb>b", 1, 1, &x);
  printf("true xor true returns: %d\n", x);

  // multiple return values
  int a = 4, b = 5;
  call_va(L, "swap", "ii>ii", a, b, &a, &b);
  printf("swapped: a=%d, b=%d\n", a, b);


  // sleep using os.sleep
  int t = 2;
  call_va(L, "sleep", "i", t);
  printf("slept for %d seconds\n", t);

  lua_close(L);
}
