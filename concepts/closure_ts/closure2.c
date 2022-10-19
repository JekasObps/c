
#include "closure.h"

DEF_CLOSURE_NO_RET(f2, int*)

void f2(int *p)
{
   (*p)++;
}

void* f1(void)
{
    static int state = 0;

    CLOSURE_TYPE(f2) *ctx = MAKE_CLOSURE(f2, &f2);
    BIND(f2, ctx, &state);

    return ctx;
}

int main (int argc, char *argv[])
{
    void* ctx = f1();
    
    CALL(f2, ctx);

    FREE_CLOSURE(ctx);

    return 0;
}
