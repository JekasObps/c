"""
    This file is a runnable python script that takes a number
    representing num of argument supported by closure.h,
    and writes expanded closure.h file to stdout.
"""

import sys
import math


def convert_decimal(num: int, base: int) -> str:
    """ Uses only letters to describe a number in specific base.
        Max available base is 26.
        o is used as 0.
    """

    table = "oabcdefghigklmnpqrstuvwxyz"
    result: str = ""

    def gen(num) -> int:
        while num != 0:
            yield math.floor(num % base)
            num //= base

    for digit in gen(num):
        result = str(table[int(digit)]) + result

    return result


def translit(num) -> str:
    return convert_decimal(num, 26)


def name(num) -> str:
    return f'_{num}'


def args(n, sep, fn) -> str:
    args = sep.join([fn(i) for i in range(1, n+1)])
    return args


def mk_list(n, fn) -> list[str]:
    return [fn(i) for i in range(1, n+1)]


def define_unwrap(n, nam) -> str:
    return f"#define _UNWRAP(M, {args(n, ', ', name)}, ARGN, ...) M##ARGN \n"


def define_list(dfn, prefix, n, typ, nam, sep, scm) -> str:
    def define(i) -> str:
        nonlocal typ
        nonlocal nam

        new_typ = typ[:i]
        new_nam = nam[:i]

        d = f"""#define {dfn}{i}({prefix + ', ' if prefix else ''} {
            ', '.join([t for t in new_typ ])}) """

        if i > 1:
            d += f"""{dfn}{i-1}({prefix + ', ' if prefix else ''} {
                ', '.join([t for t in new_typ[:i-1] ])}){sep} """

        d += f"{scm(i-1, new_typ[-1], new_nam[-1])}"

        return d

    return '\n'.join([define(i) for i in range(1, n + 1)])


def define_elipsis_list(dfn, n, typ, scm) -> str:
    def define(i) -> str:
        nonlocal typ

        new_typ = typ[:i]
        d = f"""#define {dfn}{i}({', '.join([t for t in new_typ]
                            + ['...'] if i < n+1 else [])}) {scm(new_typ)}"""
        return d
    return '\n'.join([define(i) for i in range(0, n + 1)])


def define_head(dfn, prefix, n) -> str:
    return f"""#define {dfn}({prefix + ', ' if prefix else ''}...) \\
    _UNWRAP({dfn}, __VA_ARGS__, {
        ', '.join(str(i) for i in range(n, 0, -1))
        })({prefix + ', ' if prefix else ''}__VA_ARGS__)"""


def define_indexed_head(dfn, index) -> str:
    return f"""#define {dfn}({index}, ...) {dfn}##{index}(__VA_ARGS__)"""


def define_rest() -> str:
    return """
#define COMMA() ,
#define SEMICO() ;

#define BITCOUNT(bits) \\
    static inline size_t bitcount(uint##bits##_t* bitarr, int bindc) {\\
        size_t count = 0;\\
        for (size_t i = 0; i < ((bindc + bits - 1) / bits); ++i)\\
        {\\
            uint##bits##_t word = bitarr[i];\\
            while (word)\\
            {\\
                count += word & 1;\\
                word >>= 1;\\
            }\\
        }\\
        return count;\\
    }

/* Generate all possible weak implementations for bitcount func */
BITCOUNT(8)
BITCOUNT(16)
BITCOUNT(32)
BITCOUNT(64)

#define _CALL(bindc, ...) \\
    _BOUND_ARG(bindc, _IGNORE_AFTER(bindc, __VA_ARGS__)),\\
    _IGNORE_FIRST(bindc ,##__VA_ARGS__)

#define _CLOSURE(name, bindc, bits, R, RET, ...) \\
    typedef R (*name##_fptr) (__VA_ARGS__);\\
    \\
    struct closure_##name {\\
        name##_fptr fn; \\
        \\
        struct name##_args {\\
            union {\\
                struct {\\
                    _MAP(uint##bits##_t, _IGNORE_AFTER(bindc, __VA_ARGS__));\\
                } _map;\\
                uint##bits##_t _bits[((bindc + bits - 1) / bits)];\\
            } u;\\
            _ARG(SEMICO, _IGNORE_AFTER(bindc, __VA_ARGS__));\\
        } args;\\
        \\
    };\\
    \\
    struct closure_##name * make_##name (name##_fptr fn) {\\
        struct closure_##name *ctx = calloc(1, sizeof(*ctx));\\
        \\
        if (ctx) { \\
            ctx->fn = fn;\\
        }\\
        return ctx;\\
    }\\
    \\
    _BIND(name, _IGNORE_AFTER(bindc, __VA_ARGS__))\\
    \\
    R call_##name (struct closure_##name *ctx, _IGNORE_FIRST(bindc, _ARG(COMMA, __VA_ARGS__))) {\\
        assert(bitcount(ctx->args.u._bits, bindc) == bindc && "Unbound arguments left, undefined behavior.");\\
        RET ctx->fn(_CALL(bindc, _ARG_NAME(__VA_ARGS__)));\\
    }

#define DEF_CLOSURE_RET(name, bindc, R, ...) _CLOSURE(name, bindc, 8, R, return, __VA_ARGS__) 
#define DEF_CLOSURE_NO_RET(name, bindc, ...) _CLOSURE(name, bindc, 8, void, /*no return*/ , __VA_ARGS__)

#define CLOSURE_TYPE(name) struct closure_##name
#define MAKE_CLOSURE(name, fn) make_##name(fn)
#define FREE_CLOSURE(ctx) free(ctx)
#define BIND(name, argn, ctx, value) bind_##name##_##argn(ctx, value)
#define CALL(name, ctx, ...) call_##name(ctx, ##__VA_ARGS__)

"""


def define_bind_func() -> str:
    return """
#define _BIND_FUNC(name, type, argn) \\
    void bind_##name##_##argn(struct closure_##name* ctx, type _##argn) {\\
        assert(ctx->args.u._map._##argn == 0 && "Rebinding is not allowed(argn argument)");\\
        ctx->args.u._map._##argn = 1;\\
        ctx->args._##argn = _##argn;\\
    }
\n"""


def includes() -> None:
    print("""
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>\n""")


def main(cargs) -> None:
    typ = mk_list(cargs, translit)
    nam = mk_list(cargs, name)

    print(define_unwrap(cargs, nam))

    # For list of type : _1, _2, _3, ...
    print(define_list('_ARG_NAME', None, cargs, typ, nam, ', ',
                      lambda _1, _2, m: f'{m}'))
    print(define_head('_ARG_NAME', None,  cargs), '\n')

    print(define_elipsis_list('_IGNORE_FIRST', cargs, typ,
                              lambda *_: '__VA_ARGS__'))

    print(define_indexed_head('_IGNORE_FIRST', 'bindc'), '\n')

    print(define_elipsis_list('_IGNORE_AFTER', cargs, typ,
                              lambda typ: f"{', '.join(t for t in typ)}"))

    print(define_indexed_head('_IGNORE_AFTER', 'bindc'), '\n')

    # For list of type: a _1 del() b _2 del() ...
    print(define_list('_ARG', 'del', cargs, typ, nam, ' del() ',
                      lambda _1, t, m: f'{t} {m}'))
    print(define_head('_ARG', 'del',  cargs), '\n')

    # For list of type: T _1:1; T _2:1 ...
    print(define_list('_MAP', 'T', cargs, typ, nam, '; ',
                      lambda _1, t, m: f'T {m}:1'))
    print(define_head('_MAP', 'T', cargs), '\n')

    # Bind methods
    print(define_bind_func())
    print(define_list('_BIND', 'name',  cargs, typ, nam, ' ',
                      lambda i, t, m: f'    _BIND_FUNC(name, {t}, {i+1})'))
    print(define_head('_BIND', 'name', cargs), '\n')

    print(define_list('_BOUND_ARG', None, cargs, typ, nam, ', ',
                      lambda i, t, m: f'ctx->args.{t}'))

    print(define_indexed_head('_BOUND_ARG', 'bindc'), '\n')

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
