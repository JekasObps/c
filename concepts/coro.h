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

#define _CAL1(a) a()
#define _CAL2(a, b) a(b)
#define _CAL3(a, b, c) a(b(c))
#define _CAL4(a, b, c, d) a(b(c(d)))
#define _CAL5(a, b, c, d, e) a(b(c(d(e))))
#define _CAL6(a, b, c, d, e, f) a(b(c(d(e(f)))))
#define _CAL7(a, b, c, d, e, f, g) a(b(c(d(e(f(g))))))

#define _UNWRAP(M, _1, _2, _3, _4, _5, _6, _7, ARGN, ...) M##ARGN

/* alternative syntax for chained function calls */
#define PIPE(...) _UNWRAP(_CAL, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__) 

#endif/*_CORO_H_*/
