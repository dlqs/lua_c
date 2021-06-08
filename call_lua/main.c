#include "hook.h"

int main() {

  int a = 30;
  int b = 40;
  person p = { .name = "foo", .age = 50 };
  printf("before: p.name %s\n", p.name);
  printf("before: p.age %d\n", p.age);
  printf("before: b %d\n", b);

  lua_hook("script1.lua", ("a", a), ("b", &b), ("p", &p));

  printf("after: p.name %s\n", p.name);
  printf("after: p.age %d\n", p.age);
  printf("after: b %d\n", b);
  // a remains the same

  return 0;
}
