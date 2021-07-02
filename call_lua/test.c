#include <stdio.h>

#define noop(x) _noop((void*)x)

//#define id(x) (typeof (x)) _id(typeof (x) y)

#define lambda(ret_type, _body) ({ ret_type _ _body _; })

#define id(x) lambda(typeof (x), (typeof (x) y) { return y; })(x)

void *_id(void *x) {
        return x;
}

void _noop(void *x) {}

int main() {
    char* c = "abc";
    int x = 123;
    int y = 234;
    if (id(x) == x) {
        printf("OK");
    } else {
        printf("NOT OK");
    }
    if (id(&x) == &x) {
        printf("OK");
    } else {
        printf("NOT OK");
    }
    if (id(c) == c) {
        printf("OK");
    } else {
        printf("NOT OK");
    }
}
