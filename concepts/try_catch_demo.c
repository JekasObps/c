#include "try_catch.h"

/* thrown and caught in same function */
void single_level(void)
{
    TRY 
    {
        /* your code ...  */
        THROW( EXC_ALLOC_FAIL );
    }
    CATCH( EXC_ALLOC_FAIL )
    {
       PRINT_EXC(stderr); 
    }
    FINALLY ENDTRY; /* empty finally block */
}

void func_that_throws(void)
{
    THROW( EXC_FATAL_ERROR );
}

/* throws from subsequent call */
void subsequent_call_throw(void)
{
    TRY 
    {
        func_that_throws();    
    }
    CATCH( EXC_FATAL_ERROR )
    {
       PRINT_EXC(stderr); 
    }
    FINALLY
    {
        printf("    subsequent_call_throw::FINALLY\n");
    }
    ENDTRY; 
}

void f2(void)
{
    func_that_throws();
}

void f1(void)
{
    f2();
}

/* throws from subsequent call */
void subsequent_multicalls_throw(void)
{
    TRY 
    {
        f1(); /* -> f2 -> func_that_throws */ 
    }
    CATCH( EXC_FATAL_ERROR )
    {
       PRINT_EXC(stderr);
    }
    FINALLY
    {
        printf("    subsequent_multicalls_throw::FINALLY\n");
    }
    ENDTRY; /* empty finally block */
}

void exception_causes_another_with_param(void)
{
    TRY {
        /* ... */
        THROW(EXC_RUNTIME_ERROR); /* Throwing error */
    }
    CATCH( EXC_RUNTIME_ERROR ) {
        
        /* Caused by EXC_RUNTIME_ERROR with user param 
         */
        int* param = malloc(sizeof(int));
        *param = 999;
       
#ifdef C99 /* variadic macro empty parameter list unsupported */
        THROW_PARAM(EXC_FATAL_ERROR, param); 
#else
        THROW(EXC_FATAL_ERROR, param);
#endif
    }
    FINALLY {
        printf("    exception_causes_another_with_param::FINALLY\n");
    }
    ENDTRY;
}

void handle_exception_with_param(void)
{
    TRY 
    {
        exception_causes_another_with_param();
    }
    CATCH( EXC_FATAL_ERROR )
    {
        ExData *data; 
        
        PRINT_EXC(stderr);
        
        data = ExStack.pop_exdata(); 
        printf("param: %d\n", *(int*)data->param);
        free(data->param);
    }
    FINALLY ENDTRY; /* empty finally block */
}

void cause1(void)
{
    TRY
    {
        THROW(EXC_LOGICAL_ERROR);
    }
    CATCH( EXC_LOGICAL_ERROR )
    {
        THROW(EXC_LOGICAL_ERROR);
    }
    FINALLY ENDTRY;
}

void cause2(void)
{
    TRY
    {
        cause1();
    }
    CATCH( EXC_LOGICAL_ERROR )
    {
        THROW(EXC_LOGICAL_ERROR);
    }
    FINALLY ENDTRY;
}

void cause3(void)
{
    TRY
    {
        cause2();
    }
    CATCH( EXC_LOGICAL_ERROR )
    {
        THROW(EXC_LOGICAL_ERROR);
    }
    FINALLY ENDTRY;
}

void multy_cause(void)
{
    TRY
    {
        cause3();
    }
    CATCH( EXC_LOGICAL_ERROR )
    {
        PRINT_EXC(stderr);
    }
    FINALLY ENDTRY;
}

void nested_tries(void)
{
    TRY
    {
        TRY
        {
            THROW(EXC_ALLOC_FAIL);
            /* unreached !!!*/
            THROW(EXC_RUNTIME_ERROR); 
        }
        CATCH( EXC_ALLOC_FAIL )
        {
            printf("handled in nested try\n");
            PRINT_EXC(stderr);
        }
        FINALLY printf("/nested try\n"); 
        ENDTRY;
    }
    CATCH( EXC_RUNTIME_ERROR )
    {
        printf("handled in outter try\n");
        PRINT_EXC(stderr);
    }
    FINALLY printf("/outter try\n"); 
    ENDTRY;
}


int main ()
{
    single_level();
    subsequent_call_throw();
    subsequent_multicalls_throw();
    handle_exception_with_param();
    multy_cause();
    nested_tries();

    return 0;
}
