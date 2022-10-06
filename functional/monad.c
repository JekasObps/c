#include "monad.h"

#include <stdint.h>

#define IMPL_MAYBE(type) \
    const maybe_##type Nothing_##type = (maybe_##type) {0, true}; \
    \
    maybe_##type bind_maybe_##type(maybe_##int (*fn) (type), maybe_##type m) \
    { \
        return m.nothing ? Nothing_##type : fn(m.value); \
    } \
    \
    maybe_##type return_maybe_##type(type n) { return (maybe_##type) {n, false}; }

IMPL_MAYBE(int);

maybe_int lt10_add5(int value)
{
    if (value >= 10)
        return Nothing_int;

    return return_maybe_int(value+5);
}


void func(void)
{
    /* do something */
}

void decorator(void)
{
    /* do something here */
    
    /* function to call */
    

    /* do something here */
}

int main ()
{
    maybe_int m = return_maybe_int(0);

    m = bind_maybe_int(lt10_add5, m);
    m = bind_maybe_int(lt10_add5, m);
    m = bind_maybe_int(lt10_add5, m);


    return 0;
}
