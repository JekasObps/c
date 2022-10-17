#ifndef _CORO_H_
#define _CORO_H_

/* 
 * implementation of static coroutines,
 * utilizing what's called Duff's device
 */
#define CORO_INIT do { static int _state = 0; switch(_state) { case 0: 
#define CORO_YIELD(value) do { _state = __LINE__; return value; case __LINE__:; } while (0)
#define CORO_FINI(v) }} while(0); return v
#define CORO_REWIND _state = 0
#define VOID 

#define _CAL1(ctx, a) a(ctx)
#define _CAL2(ctx, a, b) a(ctx, b)
#define _CAL3(ctx, a, b, c) a(ctx, b(ctx, c))
#define _CAL4(ctx, a, b, c, d) a(ctx, b(ctx, c(ctx, d)))
#define _CAL5(ctx, a, b, c, d, e) a(ctx, b(ctx, c(ctx, d(ctx, e))))
#define _CAL6(ctx, a, b, c, d, e, f) a(ctx, b(ctx, c(ctx, d(ctx, e(ctx, f)))))
#define _CAL7(ctx, a, b, c, d, e, f, g) a(ctx, b(ctx, c(ctx, d(ctx, e(ctx, f(ctx, g))))))

#define _UNWRAP(M, _1, _2, _3, _4, _5, _6, _7, ARGN, ...) M##ARGN

/* alternative syntax for chained function calls */
#define PIPE(ctx, ...) _UNWRAP(_CAL, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)(ctx, __VA_ARGS__) 

#endif/*_CORO_H_*/
