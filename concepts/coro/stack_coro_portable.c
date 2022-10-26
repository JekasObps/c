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
    
    size_t stack_size;
    bool returned;
    uint8_t stack_buff[0];
};

typedef struct coroutine coroutine_t;

static coroutine_t *g_coro_ptr = 0; /* transfer pointer to coro through this global variable, 
                                       only way to pass data to the handler. what if we'll made it thread_local?  */
static bool g_signal_was_set = false;

/* signal handler */
void call_coro(int sig, siginfo_t *info, void* ctx)
{
    /* printf("STACK: %s \n", (((ucontext_t*) ctx)->uc_stack.ss_flags & SS_ONSTACK)? "ALTERNATE" : "REGULAR"); */
    /* printf("Stack pointer %p \n", ((ucontext_t*) ctx)->uc_stack.ss_sp); */
    uint8_t _sentinel;
    printf("Stack pointer %p \n", &_sentinel);

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

static void set_coro_signal()
{
    if (false == g_signal_was_set)
    {
        struct sigaction sigact = {
            .sa_sigaction = call_coro,
            .sa_flags = SA_ONSTACK | SA_SIGINFO
        };

        if (0 != sigaction(SIGUSR1, &sigact, 0))
        {    
            perror("sigaction");
            return;
        }

        g_signal_was_set = true;
    }
}

static void swap_stack(stack_t* a, stack_t* b)
{
    if (0 != sigaltstack(a, b))
    {
        perror("segaltstack");
    }
}

coroutine_t* make_coro(size_t stack_size, coro_fn_t func)
{
    coroutine_t *coro = calloc(1, sizeof(coroutine_t) + stack_size);

    if (coro)
    {
        coro->func = func;
        coro->returned = false;
    
        coro->stack_size = stack_size;
        
        stack_t old_stack;
        stack_t new_stack = {
            .ss_size = coro->stack_size,
            .ss_sp = &coro->stack_buff
        };
        swap_stack(&new_stack, &old_stack);
        set_coro_signal();
        
        g_coro_ptr = coro;
        if (0 == setjmp(coro->caller))
            raise(SIGUSR1); /* setup stack */
        
        swap_stack(&old_stack, 0); 
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
    uint8_t _sentinel;
    printf("f1 sentinel %p \n", &_sentinel);
    
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

static int global_count = 0; /* another handler's data */
static void another_handler(int sig, siginfo_t *info, void* ctx)
{
    /* printf("Stack pointer %p \n", ((ucontext_t*) ctx)->uc_stack.ss_sp); */
    uint8_t _sentinel;
    printf("Stack pointer %p \n", &_sentinel);
    ++global_count;
}

static void set_another_handler()
{
     struct sigaction sigact = {
        .sa_sigaction = another_handler,
        .sa_flags = SA_SIGINFO
     };
     sigaction(SIGUSR2, &sigact, 0);
}

int main (int argc, char *argv[])
{
    coroutine_t *coro = make_coro(16*4096, foo);    
    set_another_handler();

    for (int i = 0; !resume_coro(coro); ++i)
    {
        puts("between coroutine resumes!\n");
        
        if (i > 7) 
        {
            raise(SIGUSR2); /* testing that other signals/handlers don't mess up coroutine stack  */
            printf("another handlers count = %d\n", global_count);
        }
    }

    free_coro(coro);

    return 0;
}
