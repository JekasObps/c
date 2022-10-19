
#include <ucontext.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct coroutine;
typedef void (*coro_fn_t)(struct coroutine*);

struct coroutine
{
    ucontext_t caller;
    ucontext_t callee;
    coro_fn_t func;
    bool returned;
    uint8_t stack[0];
};
typedef struct coroutine coroutine_t;

void call_coro(coroutine_t* coro)
{
    coro->func(coro);
    coro->returned = true;
}

coroutine_t* make_coro(size_t stack_sz, coro_fn_t func)
{
    coroutine_t *coro = malloc(sizeof(coroutine_t) + stack_sz);
    if (coro)
    {
        getcontext(&coro->callee);
        coro->callee.uc_link = &coro->caller;
        coro->callee.uc_stack.ss_size = stack_sz;
        coro->callee.uc_stack.ss_sp = &coro->stack;

        makecontext(&coro->callee, (void(*)(void)) &call_coro, 1, coro);
        coro->func = func;
        coro->returned = false;
    }
    return coro;
}

void resume_coro(coroutine_t* coro)
{
    if (coro->returned) return;
    swapcontext(&coro->caller, &coro->callee);
}

void yield_coro(coroutine_t* coro)
{
    swapcontext(&coro->callee, &coro->caller);
}


void free_coro(coroutine_t* coro)
{
    assert((true == coro->returned) && "coro didn't finished yet");
    memset(coro, 0, sizeof(*coro) + coro->callee.uc_stack.ss_size);
    free(coro);
}

bool coro_is_returned(coroutine_t* coro)
{
    return coro->returned;
}

void fn1(coroutine_t* coro, int a, int b)
{
    while(a > b)
    {
        a--;
        b++;
        printf("a: %d, b: %d\n", a, b);
        yield_coro(coro);
    }
}

void foo(coroutine_t *coro)
{
    for (int i = 0; i < 10; i++)
    {
        printf("coro: %d\n", i);
        yield_coro(coro);    
        fn1(coro, 4, i);
    }
    return;
}

int main (int argc, char *argv[])
{
    coroutine_t *coro = make_coro(16*4096, foo);    
    
    while (!coro_is_returned(coro))
    {
        resume_coro(coro);
        puts("between coroutine resumes!\n");
    }
    
    free_coro(coro);

    return 0;
}
