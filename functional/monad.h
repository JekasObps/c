#ifndef __MONAD_H__
#define __MONAD_H__
#include <stdbool.h>

#define DEF_MAYBE(type) \
    typedef struct maybe_##type \
    { \
        type value; \
        bool nothing; \
    } maybe_##type; \
    \
    maybe_##type bind_maybe_##type(maybe_##int (*fn) (type), maybe_##type m); \
    maybe_##type return_maybe_##type(type n)
    

DEF_MAYBE(int);

#endif /*__MONAD_H__*/



