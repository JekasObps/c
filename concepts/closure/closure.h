#ifndef _CLOSURE_H_
#define _CLOSURE_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define _ARG1(a) a _1
#define _ARG2(a, b) _ARG1(a);  b _2
#define _ARG3(a, b, c) _ARG2(a, b);  c _3
#define _ARG4(a, b, c, d) _ARG3(a, b, c);  d _4
#define _ARG5(a, b, c, d, e) _ARG4(a, b, c, d);  e _5
#define _ARG6(a, b, c, d, e, f) _ARG5(a, b, c, d, e);  f _6
#define _ARG7(a, b, c, d, e, f, g) _ARG6(a, b, c, d, e, f);  g _7
#define _ARG8(a, b, c, d, e, f, g, h) _ARG7(a, b, c, d, e, f, g);  h _8

#define _BIND1(a) case 0: ctx->_1 = va_arg(list, a)
#define _BIND2(a, b) _BIND1(a); break; case 1: ctx->_2 = va_arg(list, b)
#define _BIND3(a, b, c) _BIND2(a, b); break; case 2: ctx->_3 = va_arg(list, c)
#define _BIND4(a, b, c, d) _BIND3(a, b, c); break; case 3: ctx->_4 = va_arg(list, d)
#define _BIND5(a, b, c, d, e) _BIND4(a, b, c, d); break; case 4: ctx->_5 = va_arg(list, e)
#define _BIND6(a, b, c, d, e, f) _BIND5(a, b, c, d, e); break; case 5: ctx->_6 = va_arg(list, f)
#define _BIND7(a, b, c, d, e, f, g) _BIND6(a, b, c, d, e, f); break; case 6: ctx->_7 = va_arg(list, g)
#define _BIND8(a, b, c, d, e, f, g, h) _BIND7(a, b, c, d, e, f, g); break; case 7: ctx->_8 = va_arg(list, h)

#define _CALL1(a) case 0: ctx_copy._1 = va_arg(list, a)
#define _CALL2(a, b) _CALL1(a); /*fallthrough*/ case 1: ctx_copy._2 = va_arg(list, b)
#define _CALL3(a, b, c) _CALL2(a, b); /*fallthrough*/ case 2: ctx_copy._3 = va_arg(list, c)
#define _CALL4(a, b, c, d) _CALL3(a, b, c); /*fallthrough*/ case 3: ctx_copy._4 = va_arg(list, d)
#define _CALL5(a, b, c, d, e) _CALL4(a, b, c, d); /*fallthrough*/ case 4: ctx_copy._5 = va_arg(list, e)
#define _CALL6(a, b, c, d, e, f) _CALL5(a, b, c, d, e); /*fallthrough*/ case 5: ctx_copy._6 = va_arg(list, f)
#define _CALL7(a, b, c, d, e, f, g) _CALL6(a, b, c, d, e, f); /*fallthrough*/ case 6: ctx_copy._7 = va_arg(list, g)
#define _CALL8(a, b, c, d, e, f, g, h) _CALL7(a, b, c, d, e, f, g); /*fallthrough*/ case 7: ctx_copy._8 = va_arg(list, h)

#define _FN1(a) ctx_copy._1
#define _FN2(a, b) _FN1(a),  ctx_copy._2
#define _FN3(a, b, c) _FN2(a, b),  ctx_copy._3
#define _FN4(a, b, c, d) _FN3(a, b, c),  ctx_copy._4
#define _FN5(a, b, c, d, e) _FN4(a, b, c, d),  ctx_copy._5
#define _FN6(a, b, c, d, e, f) _FN5(a, b, c, d, e),  ctx_copy._6
#define _FN7(a, b, c, d, e, f, g) _FN6(a, b, c, d, e, f),  ctx_copy._7
#define _FN8(a, b, c, d, e, f, g, h) _FN7(a, b, c, d, e, f, g),  ctx_copy._8

#define _UNWRAP(M, _1, _2, _3, _4, _5, _6, _7, _8, ARGN, ...) M##ARGN 

#define _ARG(...) _UNWRAP(_ARG, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)
#define _BIND(...) _UNWRAP(_BIND, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)
#define _CALL(...) _UNWRAP(_CALL, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)
#define _FN(...) _UNWRAP(_FN, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)

#define _CLOSURE(name, R, RET, ...) \
    typedef R (*name##_fptr) (__VA_ARGS__); \
    struct closure_##name { name##_fptr fn; char argc; _ARG(__VA_ARGS__); }; \
    struct closure_##name * make_##name (name##_fptr fn) { \
        struct closure_##name *ctx = calloc(1, sizeof(*ctx)); \
        if (ctx) { ctx->fn = fn; } \
        return ctx; \
    } \
    void bind_##name (struct closure_##name *ctx, ...) { \
        va_list list; va_start(list, ctx); \
        switch(ctx->argc++) {_BIND(__VA_ARGS__);} \
        va_end(list);     } \
    R call_##name(struct closure_##name *ctx, ...) { \
        struct closure_##name ctx_copy = *ctx; \
        va_list list; \
        va_start(list, ctx); \
        \
        switch(ctx_copy.argc) {_CALL(__VA_ARGS__);/*fallthrough*/} \
        va_end(list); \
        RET ctx_copy.fn(_FN(__VA_ARGS__)); \
    }

#define DEF_CLOSURE_RET(name, R, ...) _CLOSURE(name, R, return, __VA_ARGS__) 
#define DEF_CLOSURE_NO_RET(name, ...) _CLOSURE(name, void, /*no return*/ , __VA_ARGS__)

#define CLOSURE_TYPE(name) struct closure_##name
#define MAKE_CLOSURE(name, fn) make_##name(fn)
#define FREE_CLOSURE(ctx) free(ctx)
#define BIND(name, ctx, arg) bind_##name(ctx, arg)
#define CALL(name, ctx, ...) call_##name(ctx, ##__VA_ARGS__)

#endif/*_CLOSURE_H_*/
