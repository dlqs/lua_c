#include "hook.h"

int main() {

  int a = 30;
  int b = 40;
  int n = 5;

  int ret = frrscript_call("factorial", ("n", &n));
  printf("n:%d\n", n);
  printf("a:%d\n", a);
  printf("b:%d\n", b);
  

  return 0;
}
