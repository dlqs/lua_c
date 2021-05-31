#ifndef DEBUG_H
#define DEBUG_H

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <stdio.h>
#include <string.h>

void stackDump(lua_State *L);

#endif // DEBUG_H
