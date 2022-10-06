#ifndef __TRY_CATCH__
#define __TRY_CATCH__

#include <stddef.h>
#include <setjmp.h>
#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define TRY_STACK_SIZE 16

#define TRY do { switch (setjmp(ExStack.try())){ case 0: while(1) {
#define CATCH(e) break; case e: ExStack.catch(e);
       
#if __STDC_VERSION__ == 199901L /* ISO C99 */
#  define C99
#  define THROW(e) ExStack.push_exdata(e, #e, __FILE__, __LINE__, 0); ExStack.throw(e)
#  define THROW_PARAM(e, param) ExStack.push_exdata(e, #e, __FILE__, __LINE__, param); ExStack.throw(e)
#else /* ISO C11+ */
#  define THROW(e, ...) ExStack.push_exdata(e, #e, __FILE__, __LINE__, (0,##__VA_ARGS__)); ExStack.throw(e)
#endif

#define FINALLY break; } default: 
#define PRINT_EXC(file) ExStack.print_exc(file)
#define ENDTRY ExStack.clear(); ExStack.rethrow(); }} while(0)

/* List of exception codes */
typedef enum exception_code 
{
    NO_EXCEPT = 0,
    EXC_RUNTIME_ERROR,
    EXC_LOGICAL_ERROR,
    EXC_ALLOC_FAIL,
    EXC_FATAL_ERROR
}
ExCode;

/* Data structure describes an error */
typedef struct exception_data
{
    ExCode code;
    const char* name;
    const char *file;
    int line;
    void *param; /* additional data user can pass */    
}
ExData;

/* public interface for ExStack */
struct exception_stack
{
    /* Methods */

    /* returns next try point to be set by setjmp */
    struct __jmp_buf_tag* (*try) (void);
    
    /* stores exception data in the stack */
    void (*push_exdata)(ExCode, const char*, const char*, int, void*);
   
    /* retrieves exception data from the stack, if none returns 0 */
    ExData* (*pop_exdata)(void);
    
    /* jumps to last try point, if none aborts program execution,
     * cas unhandled exception are not allowed */
    void (*throw) (ExCode);
  
    /* clear exc_curr, so an exception considered as handled 
     * and won't be rethrown by ENDTRY */
    void (*catch) (ExCode);

    /* rethrows current exception into outer try block */
    void (*rethrow)(void);

    /* print exception and causing exceptions recursively */
    void (*print_exc)(FILE *f);
    
    /* clear all exception data (all causes), 
     * if not cleaned next throw will be "caused by" all previous exceptions */
    void (*clear)(void);
};

extern thread_local struct exception_stack ExStack;


#endif/*__TRY_CATCH__*/
