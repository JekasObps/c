#include "bundle.h"

#include <stdlib.h>
#include <stdio.h>

void* make_foo(int line, int i)
{
    typedef struct
    {
        int line; 
        int i;
    } 
    bundle_t;

    bundle_t *bndl = malloc(sizeof(*bndl));
    if (bndl)
    {
        *bndl = (bundle_t){line, i};
    }

    return bndl;
}

typedef struct
{
    int line; 
    int i;
} 
bundle_t;

bundle_t create()
{
    return (bundle_t){ 99, 11 };
}

void use_foo(void *ctx)
{
    struct 
    {
        int line;
        int i;
    }
    *bndl = ctx;

    printf("Bundle: {line: %d ,  i: %i }\n", bndl->line, bndl->i); 
}


int main (int argc, char *argv[])
{
    void * ctx1 = make_foo(9, 3);
    use_foo(ctx1);
    
    free(ctx1);
    
    return 0;
}
