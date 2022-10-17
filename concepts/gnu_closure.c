#include <stdio.h>

/**
 * Try on closures using nested functions GNU extension.
 * calling nested function from outside of it's original scope is undefined behavior!
 *
 * valgrind reports memory access errors
 *
 * additionaly using this feature could cause marking stack segment as runnable, 
 * which is not Safe from cyber security perspective.
 **/

typedef void (*zfn)(int);

zfn foo(int a)
{
    void zoo(int b) {
        printf("zoo: a + b = %d\n", a + b);
    }

    return zoo;  
}

int main (int argc, char *argv[])
{
    foo(9)(11); 
    return 0;
}
