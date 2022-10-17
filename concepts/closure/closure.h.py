"""
    This file is a runnable python script that takes a number 
    representing num of argument supported by closure.h,
    and writes expanded closure.h file to stdout.
"""

import sys

def translit(num) -> str:
    table = "xabcdefghi"
    return ''.join(table[(ord(i)-ord('0'))] for i in str(num))

def name(num) -> str:
    return f'_{num}'

def args(n, sep, fn) -> str:
    args = sep.join([fn(i) for i in range(1, n+1)])
    return args

def mk_list(n, fn) -> list[str]:
    return [fn(i) for i in range(1, n+1)]

def define(dfn, n, typ, nam, sep, scm) -> str:
    typ = typ[:n]
    nam = nam[:n]
    d = f"#define {dfn}{n}({', '.join([t for t in typ ])}) "
    
    if n > 1: 
        d += f"{dfn}{n-1}({', '.join([t for t in typ[:n-1] ])}){sep} "
    
    d += f"{scm(n-1,typ[-1],nam[-1])}"

    return d

def define_list(dfn, n, typ, nam, sep, scm) -> str:
    return '\n'.join([define(dfn, i, typ, nam, sep, scm) for i in range(1, n+1)]) + '\n'

def define_unwrap(n, nam) -> str:
    return f"#define _UNWRAP(M, {args(n, ', ', name)}, ARGN, ...) M##ARGN \n"

def define_head(dfn, n) -> str:
    return f"""#define {dfn}(...) _UNWRAP({dfn}, __VA_ARGS__, {
        ', '.join(str(i) for i in range(n, 0, -1))})(__VA_ARGS__)"""

def define_rest() -> str:
    return """
#define _CLOSURE(name, R, RET, ...) \\
    typedef R (*name##_fptr) (__VA_ARGS__); \\
    struct closure_##name { name##_fptr fn; char argc; _ARG(__VA_ARGS__); }; \\
    struct closure_##name * make_##name (name##_fptr fn) { \\
        struct closure_##name *ctx = calloc(1, sizeof(*ctx)); \\
        if (ctx) { ctx->fn = fn; } \\
        return ctx; \\
    } \\
    void bind_##name (struct closure_##name *ctx, ...) { \\
        va_list list; va_start(list, ctx); \\
        switch(ctx->argc++) {_BIND(__VA_ARGS__);} \\
        va_end(list); \
    } \\
    R call_##name(struct closure_##name *ctx, ...) { \\
        struct closure_##name ctx_copy = *ctx; \\
        va_list list; \\
        va_start(list, ctx); \\
        \\
        switch(ctx_copy.argc) {_CALL(__VA_ARGS__);/*fallthrough*/} \\
        va_end(list); \\
        RET ctx_copy.fn(_FN(__VA_ARGS__)); \\
    }

#define DEF_CLOSURE_RET(name, R, ...) _CLOSURE(name, R, return, __VA_ARGS__) 
#define DEF_CLOSURE_NO_RET(name, ...) _CLOSURE(name, void, /*no return*/ , __VA_ARGS__)

#define CLOSURE_TYPE(name) struct closure_##name
#define MAKE_CLOSURE(name, fn) make_##name(fn)
#define FREE_CLOSURE(ctx) free(ctx)
#define BIND(name, ctx, arg) bind_##name(ctx, arg)
#define CALL(name, ctx, ...) call_##name(ctx, ##__VA_ARGS__)\n"""

def includes():
    print("""
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>\n""")

def main(cargs):
    typ = mk_list(cargs, translit)
    nam = mk_list(cargs, name)

    print(define_list('_ARG', cargs, typ, nam, '; ', lambda _, t, m : f'{t} {m}'))
    
    print(define_list('_BIND', cargs, typ, nam, '; break;', 
        lambda i, t, m: f'case {i}: ctx->{m} = va_arg(list, {t})'))

    print(define_list('_CALL', cargs, typ, nam, '; /*fallthrough*/', 
        lambda i, t, m: f'case {i}: ctx_copy.{m} = va_arg(list, {t})'))

    print(define_list('_FN', cargs, typ, nam, ', ', lambda i, t, m: f'ctx_copy.{m}'))

    print(define_unwrap(cargs, nam))
    print(define_head('_ARG', cargs)) 
    print(define_head('_BIND', cargs))
    print(define_head('_CALL', cargs))
    print(define_head('_FN', cargs)) 
    print(define_rest())

def header(cargs) -> None:
    print("#ifndef _CLOSURE_H_\n#define _CLOSURE_H_")
    includes()
    main(cargs)
    print("#endif/*_CLOSURE_H_*/")

def run() -> None:
    cargs = int(sys.argv[1]) if len(sys.argv) > 1 else 8
    header(cargs)

if __name__ == '__main__':
    run()
