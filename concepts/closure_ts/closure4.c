#include <stdio.h>
#include "closure.h"

typedef struct Type {
    short n; float b;
} Type;


DEF_CLOSURE_NO_RET(func, Type, Type)
void func(Type a, Type b)
{
    printf("a: %d, %f", a.n, a.b);
    printf("b: %d, %f", b.n, b.b);
}


int main (int argc, char *argv[])
{
    void *c = MAKE_CLOSURE(func, &func);
    BIND(func, c, ((Type){23, 0.1f}));
    BIND(func, c, ((Type){6553, 12.21f}));
    
    CALL(func, c);
    FREE_CLOSURE(c);

    return 0;
}
