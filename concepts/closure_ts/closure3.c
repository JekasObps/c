#include "closure.h"
#include "coro.h"

DEF_CLOSURE_RET(
    f1, int, int
)



int f1(int a)
{
    
    return a*a;
}

int main (int argc, char *argv[])
{
    CLOSURE_TYPE(f1) ctx = {
        .fn = f1,
        .argc = 1,
        ._1 = 12
    };
    
    CALL(f1, &ctx);

    return 0;
}
