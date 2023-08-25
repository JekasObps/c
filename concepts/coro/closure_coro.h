#ifndef _CORO_H_
#define _CORO_H_

#include "../closure_ts/closure.h"


/* 
 * implementation of reentrant coroutines using closures,
 * utilizing what's called Duff's device
 */
#define CORO_INIT do { switch(ctx->args._1) { case 0:
#define CORO_YIELD(value) do { ctx->args._1 = __LINE__; return value; case __LINE__:; } while (0)
#define CORO_FINI(v) }} while(0); return v
#define CORO_REWIND ctx->args._1 = 0
#define VOID

#endif/*_CORO_H_*/
