#include <stdio.h>
#include "closure.h"

typedef struct S
{
    int a;
    size_t b;
} s_t;

DEF_CLOSURE_RET(foo, 2, double, int, s_t, float, double, const char*)

DEF_CLOSURE_RET(foo2, 2, double, int, int, char*)

double foo(int a, s_t b, float c, double d, const char * str)
{
    printf("foo(%d, {a:%d, b:%lu}, %f, %lf, %s)\n", a, b.a, b.b, c, d, str);
    return (d + c / a);
}

int main (int argc, char *argv[])
{
    void* ctx = MAKE_CLOSURE(foo, &foo);    

    s_t s = {9, 19ul};

    BIND(foo, 1, ctx, 339);
    BIND(foo, 2, ctx, s);

    CALL(foo, ctx, 0.12f, 45.5, "OK dude");

    FREE_CLOSURE(ctx);

    return 0;
}

