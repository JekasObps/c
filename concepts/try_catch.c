#include "try_catch.h"

/* internals of ExStack */
struct private_exception_stack
{
    /* Exception that is currently handled */
    ExCode curr_exc;
    
    /* Stack of exception related data */
    ExData ex_data[TRY_STACK_SIZE];
    size_t ex_num;

    /* Stack of jump points to nested try constructions */
    jmp_buf tries[TRY_STACK_SIZE];
    size_t try_num;
};

static thread_local struct private_exception_stack _ExStack = {0};

/* Methods declaration */

static struct __jmp_buf_tag* try(void);

static void push_exdata(ExCode code,
    const char* name,
    const char* file, 
    int line,
    void* param);

static ExData* pop_exdata(void);

static void throw(ExCode e);

static void catch(ExCode e);

static void rethrow(void);

static bool has_try(void);

static void print_exc(FILE *f);

static void clear(void);

/* public interface */
thread_local struct exception_stack ExStack = {
    try,
    push_exdata,
    pop_exdata,
    throw,
    catch,
    rethrow,
    print_exc,
    clear
};

static struct __jmp_buf_tag* try(void)
{
    assert(("Try stack overflow", _ExStack.try_num < TRY_STACK_SIZE));
    return _ExStack.tries[_ExStack.try_num++];
}

static void push_exdata(ExCode code,
    const char* name,
    const char* file, 
    int line,
    void* param)
{
    ExData *data; 
    assert(("No try points left in the stack!", _ExStack.try_num > 0));
    
    data = &_ExStack.ex_data[_ExStack.ex_num];
    
    data->code = code;
    data->name = name;
    data->file = file;
    data->line = line;
    data->param = param;

    ++_ExStack.ex_num;
}

static ExData* pop_exdata(void)
{
    return (_ExStack.ex_num) ? &_ExStack.ex_data[--_ExStack.ex_num] : 0;
}

static void throw(ExCode e)
{
    _ExStack.curr_exc = e;
    if (has_try()) longjmp(_ExStack.tries[--_ExStack.try_num], e);
    
    /* has no tries left */
    fprintf(stderr, "Uncaught error in main function, abort!\n"); abort(); 
}

static void catch(ExCode e)
{
    assert(("Catched exception has to be match for current one!", _ExStack.curr_exc == e));
    _ExStack.curr_exc = NO_EXCEPT;
}

static void rethrow(void)
{
    if(_ExStack.curr_exc) throw(_ExStack.curr_exc);
}

static bool has_try()
{
    return !!_ExStack.try_num; 
}

static void print_exc(FILE *f)
{
    /* get current data */
    size_t num = _ExStack.ex_num; 
    ExData *data = &_ExStack.ex_data[--num];

    fprintf(f, "%s(%d) in %s at line #%d\n", data->name, data->code, data->file, data->line);
    
    while (num)
    {
        data = &_ExStack.ex_data[--num];
        fprintf(f, " --- caused by %s(%d) in %s at line #%d\n", data->name, data->code, data->file, data->line);
    }
}

static void clear(void)
{
    /* clear exception data if handled */
    if (_ExStack.curr_exc == NO_EXCEPT)
        _ExStack.ex_num = 0;
}   
