
#include <ucontext.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/***
 *
 *  Example of coroutine that returns value(double) on yield and
 *  takes parameter(int) on resume 
 *
 *  simple. just adding these variables to a coroutine struct + modifying 
 *  methods depending on parameter.
 *
 *  This can be easily converted to some sort of META template MACROS.
 *
 ***/


struct coroutine;
typedef void (*coro_fn_t)(struct coroutine*);

struct coroutine
{
    ucontext_t caller;
    ucontext_t callee;
    coro_fn_t func;
    bool returned;  
    int param;      /* param that is passed on resume */
    double value;   /* yield value */
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

double resume_coro_double(coroutine_t* coro, int param)
{
    coro->param = param;
    swapcontext(&coro->caller, &coro->callee);
    
    return coro->value;
}

void yield_coro_double(coroutine_t* coro, double value)
{
    coro->value = value; 
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


/**
 * Example of user defined cooperative methods.
 * these are foo that calls normally to fn1, and both yield back to caller
 **/

void fn1(coroutine_t* coro, int a, int b)
{
    while(a > b)
    {
        a--;
        b++;
        printf("a: %d, b: %d\n", a, b);
        yield_coro_double(coro, a + b);
    }
}

void foo(coroutine_t *coro)
{
    for (int i = 0; i < 10; i++)
    {
        printf("coro: %d, param: %d\n", i, coro->param);
        yield_coro_double(coro, i*i/3);
        fn1(coro, 4, i);
    }
    return;
}

int main (int argc, char *argv[])
{
    coroutine_t *coro = make_coro(16*4096, foo);    
   
    for (int i = 20; !coro_is_returned(coro); --i)
    {
        double d = resume_coro_double(coro, i);
        printf("between coroutine resumes! value: %f\n", d);
    }
    
    free_coro(coro);

    return 0;
}
