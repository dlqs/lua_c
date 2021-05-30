#ifndef REPL_H
#define REPL_H

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <stdio.h>
#include <string.h>

void repl(lua_State *L);

#endif // REPL_H
