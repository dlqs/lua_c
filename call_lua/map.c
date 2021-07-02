#include <stdio.h>
#define _CONCAT2(a, b) a##b
#define _CONCAT(a, b) _CONCAT2(a, b)

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


void f(int x, int y) {
    printf("%d\n", x + y);
}
int main() {
    MAP_LISTS(f, (1, 2), (3, 4));
}
