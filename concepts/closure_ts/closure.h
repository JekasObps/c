#ifndef _CLOSURE_H_
#define _CLOSURE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define _UNWRAP(M, _1, _2, _3, _4, _5, _6, ARGN, ...) M##ARGN 

#define _ARG_NAME1( a) _1
#define _ARG_NAME2( a, b) _ARG_NAME1( a),  _2
#define _ARG_NAME3( a, b, c) _ARG_NAME2( a, b),  _3
#define _ARG_NAME4( a, b, c, d) _ARG_NAME3( a, b, c),  _4
#define _ARG_NAME5( a, b, c, d, e) _ARG_NAME4( a, b, c, d),  _5
#define _ARG_NAME6( a, b, c, d, e, f) _ARG_NAME5( a, b, c, d, e),  _6
#define _ARG_NAME(...) \
    _UNWRAP(_ARG_NAME, __VA_ARGS__, 6, 5, 4, 3, 2, 1)(__VA_ARGS__) 

#define _IGNORE_FIRST0(...) __VA_ARGS__
#define _IGNORE_FIRST1(a, ...) __VA_ARGS__
#define _IGNORE_FIRST2(a, b, ...) __VA_ARGS__
#define _IGNORE_FIRST3(a, b, c, ...) __VA_ARGS__
#define _IGNORE_FIRST4(a, b, c, d, ...) __VA_ARGS__
#define _IGNORE_FIRST5(a, b, c, d, e, ...) __VA_ARGS__
#define _IGNORE_FIRST6(a, b, c, d, e, f, ...) __VA_ARGS__
#define _IGNORE_FIRST(bindc, ...) _IGNORE_FIRST##bindc(__VA_ARGS__) 

#define _IGNORE_AFTER0(...) 
#define _IGNORE_AFTER1(a, ...) a
#define _IGNORE_AFTER2(a, b, ...) a, b
#define _IGNORE_AFTER3(a, b, c, ...) a, b, c
#define _IGNORE_AFTER4(a, b, c, d, ...) a, b, c, d
#define _IGNORE_AFTER5(a, b, c, d, e, ...) a, b, c, d, e
#define _IGNORE_AFTER6(a, b, c, d, e, f, ...) a, b, c, d, e, f
#define _IGNORE_AFTER(bindc, ...) _IGNORE_AFTER##bindc(__VA_ARGS__) 

#define _ARG1(del,  a) a _1
#define _ARG2(del,  a, b) _ARG1(del,  a) del()  b _2
#define _ARG3(del,  a, b, c) _ARG2(del,  a, b) del()  c _3
#define _ARG4(del,  a, b, c, d) _ARG3(del,  a, b, c) del()  d _4
#define _ARG5(del,  a, b, c, d, e) _ARG4(del,  a, b, c, d) del()  e _5
#define _ARG6(del,  a, b, c, d, e, f) _ARG5(del,  a, b, c, d, e) del()  f _6
#define _ARG(del, ...) \
    _UNWRAP(_ARG, __VA_ARGS__, 6, 5, 4, 3, 2, 1)(del, __VA_ARGS__) 

#define _MAP1(T,  a) T _1:1
#define _MAP2(T,  a, b) _MAP1(T,  a);  T _2:1
#define _MAP3(T,  a, b, c) _MAP2(T,  a, b);  T _3:1
#define _MAP4(T,  a, b, c, d) _MAP3(T,  a, b, c);  T _4:1
#define _MAP5(T,  a, b, c, d, e) _MAP4(T,  a, b, c, d);  T _5:1
#define _MAP6(T,  a, b, c, d, e, f) _MAP5(T,  a, b, c, d, e);  T _6:1
#define _MAP(T, ...) \
    _UNWRAP(_MAP, __VA_ARGS__, 6, 5, 4, 3, 2, 1)(T, __VA_ARGS__) 


#define _BIND_FUNC(name, type, argn) \
    void bind_##name##_##argn(struct closure_##name* ctx, type _##argn) {\
        assert(ctx->args.u._map._##argn == 0 && "Rebinding is not allowed(argn argument)");\
        ctx->args.u._map._##argn = 1;\
        ctx->args._##argn = _##argn;\
    }


#define _BIND1(name,  a)     _BIND_FUNC(name, a, 1)
#define _BIND2(name,  a, b) _BIND1(name,  a)      _BIND_FUNC(name, b, 2)
#define _BIND3(name,  a, b, c) _BIND2(name,  a, b)      _BIND_FUNC(name, c, 3)
#define _BIND4(name,  a, b, c, d) _BIND3(name,  a, b, c)      _BIND_FUNC(name, d, 4)
#define _BIND5(name,  a, b, c, d, e) _BIND4(name,  a, b, c, d)      _BIND_FUNC(name, e, 5)
#define _BIND6(name,  a, b, c, d, e, f) _BIND5(name,  a, b, c, d, e)      _BIND_FUNC(name, f, 6)
#define _BIND(name, ...) \
    _UNWRAP(_BIND, __VA_ARGS__, 6, 5, 4, 3, 2, 1)(name, __VA_ARGS__) 

#define _BOUND_ARG1( a) ctx->args.a
#define _BOUND_ARG2( a, b) _BOUND_ARG1( a),  ctx->args.b
#define _BOUND_ARG3( a, b, c) _BOUND_ARG2( a, b),  ctx->args.c
#define _BOUND_ARG4( a, b, c, d) _BOUND_ARG3( a, b, c),  ctx->args.d
#define _BOUND_ARG5( a, b, c, d, e) _BOUND_ARG4( a, b, c, d),  ctx->args.e
#define _BOUND_ARG6( a, b, c, d, e, f) _BOUND_ARG5( a, b, c, d, e),  ctx->args.f
#define _BOUND_ARG(bindc, ...) _BOUND_ARG##bindc(__VA_ARGS__) 


#define COMMA() ,
#define SEMICO() ;

#define BITCOUNT(bits) \
    static inline size_t bitcount(uint##bits##_t* bitarr, int bindc) {\
        size_t count = 0;\
        for (size_t i = 0; i < ((bindc + bits - 1) / bits); ++i)\
        {\
            uint##bits##_t word = bitarr[i];\
            while (word)\
            {\
                count += word & 1;\
                word >>= 1;\
            }\
        }\
        return count;\
    }

/* Generate all possible weak implementations for bitcount func */
BITCOUNT(8)
BITCOUNT(16)
BITCOUNT(32)
BITCOUNT(64)

#define _CALL(bindc, ...) \
    _BOUND_ARG(bindc, _IGNORE_AFTER(bindc, __VA_ARGS__)),\
    _IGNORE_FIRST(bindc ,##__VA_ARGS__)

#define _CLOSURE(name, bindc, bits, R, RET, ...) \
    typedef R (*name##_fptr) (__VA_ARGS__);\
    \
    struct closure_##name {\
        name##_fptr fn; \
        \
        struct name##_args {\
            union {\
                struct {\
                    _MAP(uint##bits##_t, _IGNORE_AFTER(bindc, __VA_ARGS__));\
                } _map;\
                uint##bits##_t _bits[((bindc + bits - 1) / bits)];\
            } u;\
            _ARG(SEMICO, _IGNORE_AFTER(bindc, __VA_ARGS__));\
        } args;\
        \
    };\
    \
    struct closure_##name * make_##name (name##_fptr fn) {\
        struct closure_##name *ctx = calloc(1, sizeof(*ctx));\
        \
        if (ctx) { \
            ctx->fn = fn;\
        }\
        return ctx;\
    }\
    \
    _BIND(name, _IGNORE_AFTER(bindc, __VA_ARGS__))\
    \
    R call_##name (struct closure_##name *ctx, _IGNORE_FIRST(bindc, _ARG(COMMA, __VA_ARGS__))) {\
        assert(bitcount(ctx->args.u._bits, bindc) == bindc && "Unbound arguments left, undefined behavior.");\
        RET ctx->fn(_CALL(bindc, _ARG_NAME(__VA_ARGS__)));\
    }

#define DEF_CLOSURE_RET(name, bindc, R, ...) _CLOSURE(name, bindc, 8, R, return, __VA_ARGS__) 
#define DEF_CLOSURE_NO_RET(name, bindc, ...) _CLOSURE(name, bindc, 8, void, /*no return*/ , __VA_ARGS__)

#define CLOSURE_TYPE(name) struct closure_##name
#define MAKE_CLOSURE(name, fn) make_##name(fn)
#define FREE_CLOSURE(ctx) free(ctx)
#define BIND(name, argn, ctx, value) bind_##name##_##argn(ctx, value)
#define CALL(name, ctx, ...) call_##name(ctx, ##__VA_ARGS__)


#endif/*_CLOSURE_H_*/
