#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 * 
 * Separate stack coroutines implementation using setjmp and signals 
 *  my be more portable and may run on windows (I hope)
 *  
 **/

struct coroutine;
typedef void (*coro_fn_t)(struct coroutine*);

struct coroutine
{
    jmp_buf caller;
    jmp_buf callee;

    coro_fn_t func;

    bool returned;
    uint8_t stack_buff[0];
};

typedef struct coroutine coroutine_t;

static coroutine_t *g_coro_ptr = 0; /* transfer pointer to coro through this global variable, 
                                       only way to pass data to the handler. what if we'll made it thread_local?  */

/* signal handler */
void call_coro(int sig, siginfo_t *info, void* context)
{
    /* prepare stack */
    coroutine_t *coro = g_coro_ptr;
    
    if(0 == sigsetjmp(coro->callee, 0))
    {
        /* preparation completed drop back to main thread */
        longjmp(coro->caller, 1);
    }

    coro->func(coro);
    coro->returned = true;
    longjmp(coro->caller, 1); /* have to exit with longjmp, to prevent stack smashing 
                                 because we raised signal in a function make_coro that is no longer */
}

coroutine_t* make_coro(size_t stack_sz, coro_fn_t func)
{
    coroutine_t *coro = calloc(1, sizeof(coroutine_t) + stack_sz);
    if (coro)
    {
        struct sigaction sigact = {
            .sa_sigaction = call_coro
        };
        sigaction(SIGUSR1, &sigact, 0);
        
        coro->func = func;
        coro->returned = false;
        
        stack_t stack = {
            .ss_size = stack_sz,
            .ss_sp = &coro->stack_buff,
            .ss_flags = SS_ONSTACK
        };
        sigaltstack(&stack, 0);
        
        g_coro_ptr = coro;

        if (0 == setjmp(coro->caller))
            raise(SIGUSR1); /* setup stack */
    }

    return coro;
}

bool resume_coro(coroutine_t* coro)
{
    if (0 == setjmp(coro->caller))
        longjmp(coro->callee, 1);

    return coro->returned;
}

void yield_coro(coroutine_t* coro)
{
    if (0 == setjmp(coro->callee))
        longjmp(coro->caller, 1);
}

void free_coro(coroutine_t* coro)
{
    assert((true == coro->returned) && "coro didn't finished yet");
    /* memset(coro, 0, sizeof(*coro)); */
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
}

int main (int argc, char *argv[])
{
    coroutine_t *coro = make_coro(16*4096, foo);    
    
    while ( !resume_coro(coro))
    {
        puts("between coroutine resumes!\n");
    }
    
    free_coro(coro);

    return 0;
}
