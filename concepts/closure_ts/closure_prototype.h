#ifndef _CLOSURE_H_
#define _CLOSURE_H_

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Evgenii Semenov
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define _UNWRAP(M, _1, _2, _3, _4, _5, _6, _7, ARGN, ...) M##ARGN

#define _ARG1(a) a _1 
#define _ARG2(a, b) a _1; b _2
#define _ARG3(a, b, c) _ARG2(a, b); c _3
#define _ARG4(a, b, c, d) _ARG3(a, b, c); d _4
#define _ARG5(a, b, c, d, e) _ARG4(a, b, c, d); e _5
#define _ARG6(a, b, c, d, e, f) _ARG5(a, b, c, d, e); f _6
#define _ARG7(a, b, c, d, e, f, g) _ARG6(a, b, c, d, e, f); g _7

#define _ARGS(...) _UNWRAP(_ARG, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__) 

#define _CASE(c, ac, m, t) case c: ac m = va_arg(list, t)
#define _BIND1(a) _CASE(0, ctx->, _1, a)
#define _BIND2(a, b) _BIND1(a); break; _CASE(1, ctx->, _2, b)
#define _BIND3(a, b, c) _BIND2(a, b); break; _CASE(2, ctx->, _3, c)
#define _BIND4(a, b, c, d) _BIND3(a, b, c); break; _CASE(3, ctx->, _4, d)
#define _BIND5(a, b, c, d, e) _BIND4(a, b, c, d); break; _CASE(4, ctx->, _5, e)
#define _BIND6(a, b, c, d, e, f) _BIND5(a, b, c, d, e); break; _CASE(5, ctx->, _6, f)
#define _BIND7(a, b, c, d, e, f, g) _BIND6(a, b, c, d, e, f); break; _CASE(6, ctx->, _7, g)

#define _BIND(...) _UNWRAP(_BIND, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__) 

#define _ARG_MAX(...) _UNWRAP(, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)

#define _CALL1(a) _CASE(0, ctx_copy., _1, a)  
#define _CALL2(a, b) _CALL1(a); /*fallthrough*/_CASE(1, ctx_copy., _2, b)  
#define _CALL3(a, b, c) _CALL2(a, b); /*fallthrough*/_CASE(2, ctx_copy., _3, c)
#define _CALL4(a, b, c, d) _CALL3(a, b, c); /*fallthrough*/_CASE(3, ctx_copy., _4, d) 
#define _CALL5(a, b, c, d, e) _CALL4(a, b, c, d); /*fallthrough*/_CASE(4, ctx_copy., _5, e)
#define _CALL6(a, b, c, d, e, f) _CALL5(a, b, c, d, e); /*fallthrough*/_CASE(5, ctx_copy., _6, f)
#define _CALL7(a, b, c, d, e, f, g) _CALL6(a, b, c, d, e, f); /*fallthrough*/_CASE(6, ctx_copy., _7, g) 
#define _CALL(...) _UNWRAP(_CALL, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)

#define _FN1 ctx_copy._1
#define _FN2 _FN1, ctx_copy._2
#define _FN3 _FN2, ctx_copy._3
#define _FN4 _FN3, ctx_copy._4
#define _FN5 _FN4, ctx_copy._5
#define _FN6 _FN5, ctx_copy._6
#define _FN7 _FN6, ctx_copy._7
#define _FN(...) _UNWRAP(_FN, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)

#define _CLOSURE(name, R, RET, ...) \
    typedef R (*name##_fptr) (__VA_ARGS__); \
    struct closure_##name { name##_fptr fn; char argc; _ARGS(__VA_ARGS__); }; \
    struct closure_##name * make_##name (name##_fptr fn) { \
        struct closure_##name *ctx = calloc(1, sizeof(*ctx)); \
        if (ctx) { ctx->fn = fn; } \
        return ctx; \
    } \
    void bind_##name (struct closure_##name *ctx, ...) { \
        va_list list; va_start(list, ctx); \
        switch(ctx->argc++) {_BIND(__VA_ARGS__);} \
        va_end(list); \
    } \
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
