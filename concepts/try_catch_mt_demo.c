#include "try_catch.h"

int thread_func(void *param)
{
    TRY 
    {
        printf("entering %s\n", (const char*) param);
    
        thrd_sleep(&(struct timespec){.tv_nsec = 200000}, 0);
        THROW(EXC_ALLOC_FAIL);
    }
    CATCH (EXC_ALLOC_FAIL)
    {
        printf("catch %s\n", (const char*) param);
        PRINT_EXC(stderr); 
    }
    FINALLY
    ENDTRY;

    return thrd_success;
}

int thread_func2(void *param)
{        
    printf("entering %s\n", (const char*) param);
    THROW(EXC_ALLOC_FAIL);

    return thrd_success;
}

int main (int argc, char *argv[])
{
    thrd_t thread1, thread2;
    
    thrd_create(&thread1, thread_func, "Thread-1");
    thrd_sleep(&(struct timespec){.tv_nsec = 100000}, 0);
    thrd_create(&thread2, thread_func2, "Thread-2");

    thrd_join(thread1, 0);
    thrd_join(thread2, 0);

    return 0;
}
