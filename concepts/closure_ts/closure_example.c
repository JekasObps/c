#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

/***** 
 *
 *   DEF_CLOSURE_[NO]RET(foo, 2, int, double, char, const char*) 
 *   
 *   name                         - name that will become prefix to all closure functions
 *
 *   bind_count                   - number of arguments that we are going to pass via closure, 
 *                                  this is fixed amount
 *
 *   [Return value type], ...rest - argument types list
 *
 *****/

/* any function that returns double and takes (int, double, char, const char *) */
typedef double (*foo_t)(int, double, char, const char *);

struct closure_foo 
{
    foo_t fn;     /* pointer to that specific function */
    
    struct foo_args
    {
        uint8_t _map; /* bitarray that stores information about what args already bound */ 
        int     _1;
        double  _2;
        /* ... */
    } 
    args;
};

struct closure_foo* make_foo(foo_t fn)
{
    struct closure_foo* ctx = calloc(1, sizeof(*ctx));
    
    if (ctx)
    {
        ctx->fn = fn;
    }

    return ctx;
}

void bind_foo_1(struct closure_foo* ctx, int _1)
{
    assert((ctx->args._map & 0x1) == 0 && "rebinding of the first argument");
    ctx->args._map |= 0x1;
    ctx->args._1 = _1;
}

void bind_foo_2(struct closure_foo* ctx, double _2)
{
    assert((ctx->args._map & 0x2) == 0 && "rebinding of the second argument");
    ctx->args._map |= 0x2;
    ctx->args._2 = _2;
}

double call_foo(struct closure_foo* ctx, char _3, const char* _4)
{
    assert(ctx->args._map == (0x1|0x2) && "unbound arguments still present, unable to call");
    return ctx->fn(ctx->args._1, ctx->args._2, _3, _4);
}

double foo(int a, double b, char c, const char* s)
{
    printf("%d %f %c %s\n", a, b, c, s);
    return b;
}

int main(void) 
{
    struct closure_foo *ctx = make_foo(foo);
   
    bind_foo_1(ctx, 333);
    bind_foo_2(ctx, 2.3);

    call_foo(ctx, 'A', "Hello World");
    
    free(ctx);

	return 0;
}

