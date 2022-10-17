#include "closure.h"
#include <stdio.h>

/* closure that reproduces itself */
DEF_CLOSURE_RET(f1, void*, int, double)

/* function that returns new closure with bound parameters */
void* f1(int a, double b)
{
    void *ctx = MAKE_CLOSURE(f1, &f1); /* closure takes same f1 function pointer */

    BIND(f1, ctx, a);
    BIND(f1, ctx, b);

    return ctx;
}

void f2(void *ctx, const char* fmt)
{
    CLOSURE_TYPE(f1) *c = ctx;
    printf(fmt, c->_1, c->_2);
    FREE_CLOSURE(ctx);
    return;
}

/* just for fun replicating oop-style syntax */
#define F2(c) c )
#define F1(a, b) f2(f1(a, b), F2

int main (int argc, char *argv[])
{
    /* f2(f1(12, 23), "%d %f\n"); */
    F1(12, 32)("%d %f\n"); /* so smooth */
    
    /* first time f1 called */
    void *ctx1 = f1(1, 2.0);
    /* every time CALL is used on that closure, it basically replicates itself */
    void* ctx2 = CALL(f1, ctx1);
    void *ctx3 = CALL(f1, ctx2);

    /* then f2 will consume each one of that closures, 
     * note that it does closure free */

    f2(ctx1, "%0.s %0.s\n");
    f2(ctx2, "%d %2.2f\n");
    f2(ctx3, "%d %0.0f\n");

    return 0;
}
