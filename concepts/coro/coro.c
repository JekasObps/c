#include "coro.h"
#include <stdio.h>

int generator(int start, int end)
{
    static int i;

    CORO_INIT; 
    for (i = start; i < end; ++i)
    {
        CORO_YIELD(i);   
    }
    CORO_REWIND;
    CORO_FINI(end);
}

int accumulate(int i)
{
    static int acc = 0;

    CORO_INIT;
    while(i != -1) CORO_YIELD(acc+=i);
    CORO_FINI(acc);
}

void consume(int i)
{
    printf("%d\n", i);
}

int main (int argc, char *argv[])
{
    for (int i = 0; i < 20; ++i)
        PIPE(VOID, consume, accumulate, generator(3, 7));  //consume(accumulate(generator(3, 7)));
   
    return 0;
}
