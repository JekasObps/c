#include <stdio.h>
#include <string.h>


/* any function that returns double and takes (int, double, char) */
typedef double (*calc_t)(int a, double b, char c);

struct closure_Didc;

struct closure_Didc 
{
    calc_t fn; /* pointer to that specific function */
    char argc; /* count of argument binding */

    /* arguments them selves */
    int     a;
    double  b;
    char    c; 
};

struct closure_Didc* make_Didc(calc_t fn)
{
    struct closure_Didc* cl = calloc(1, sizeof(*cl));
    
    if (cl)
    {
        cl->fn = fn;
    }

    return cl;
}

void bind_Didc(struct closure_Didc* ctx, ...)
{
    va_list list;
    va_start(list, ctx);
    
    switch(ctx->argc++)
    {
        case 0: ctx->a = va_arg(list, int);
        break;
        
        case 1: ctx->b = va_arg(list, double);
        break;
    
        case 2: ctx->c = (char) va_arg(list, int);
    }

    va_end(list);
}

double call_Didc_3(struct closure_Didc* ctx, double a, double b, char c)
{
    assert(ctx->argc == 0 && "bound argument count missmatch!");
    return ctx->fn(a, b, c);
}

double call_Didc_2(struct closure_Didc* ctx, double b, char c)
{
    assert(ctx->argc == 1 && "bound argument count missmatch!");
    return ctx->fn(ctx->a, b, c);
}

double call_Didc_1(struct closure_Didc* ctx, char c)
{
    assert(ctx->argc == 2 && "bound argument count missmatch!");
    return ctx->fn(ctx->a, ctx->b, c);
}

double call_Didc_0(struct closure_Didc* ctx)
{
    assert(ctx->argc == 3 && "bound argument count missmatch!");
    return ctx->fn(ctx->a, ctx->b, ctx->c);
}

double call_Didc(struct closure_Didc* ctx, ...)
{
    struct closure_Didc c = *ctx;    
    va_list list;
    va_start(list, ctx);

    switch(3 - c.argc)
    {
        case 3: c.a = va_arg(list, int);
        /* fallthrough */ 
        case 2: c.b = va_arg(list, double);
        /* fallthrough */
        case 1: c.c = (char) va_arg(list, int);
        /* fallthrough */
    }

    va_end(list);
    return c.fn(c.a, c.b, c.c);
}

double foo(int a, double b, char c)
{
    printf("%d %f %c\n", a, b, c);
    return b;
}

int main(void) 
{
    struct closure_Didc *ctx = make_Didc(foo);
    
    bind_Didc(ctx, 1);
    bind_Didc(ctx, 2.0);
    bind_Didc(ctx, 'A');
    
    double res = call_Didc_0(ctx);

    free(ctx);

	return 0;
}

