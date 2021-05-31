#include "interpreter.h"

void interpreter() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  char buff[256];
  int error;
  while (fgets(buff, sizeof(buff), stdin) != NULL) {
    error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
    if (error) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      lua_pop(L, 1); /* pop error message from the stack */
    }
  }
  lua_close(L);
}

int main() {
  interpreter();
}
