#include "hook.h"

int main() {
  printf(" === script1.lua === \n");
  int a = 100;
  int b = 200;
  lua_hook("script1.lua", a, b);
  printf(" === done === \n");

  printf(" === script2.lua === \n");
  int c = 100;
  double d = 2.0;
  lua_hook("script2.lua", c, d);
  printf(" === done === \n");

  /*
   lua_hook cannot take char as its arg, since it will not be able to find an encoder for it.

   the following will not compile:

   char e = 'E';
   lua_hook("script2.lua", e);
   */

  printf(" === script3.lua (does not exist) === \n");
  lua_hook("script3.lua");
  printf(" === done === \n");
  return 0;
}
