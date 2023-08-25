#include <stdio.h>
#include "closure_coro.h"


DEF_CLOSURE_RET(coro, 2, double, int, int)

double coroutine(CLOSURE_TYPE(coro) *ctx)
{
    CORO_INIT;
    for (ctx->args._2 = 0; 
        ctx->args._2 < 10; 
        ++ctx->args._2)
    {
        CORO_YIELD(ctx->args._2 * 1.5);
    }
    CORO_REWIND;
    CORO_FINI(0);
}


int main (int argc, char *argv[])
{
    
    void * ctx = MAKE_CLOSURE(coro, &coro);  


    return 0;
}
