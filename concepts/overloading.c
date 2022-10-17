/**
 *
 * Function overloading with C11 _Generic 
 *
 **/
#include <stddef.h>
#include <stdio.h>

#include <stdint.h>
#include <threads.h>
#include <stdlib.h>

void foo_char_ptr(const char *ptr)
{
    printf("%s\n", ptr); 
}

void foo_int(int n)
{
    printf("%d\n", n);
}

#define foo(x) _Generic((x), \
        char* : foo_char_ptr, \
        int : foo_int)(x)

typedef void (*fn)(int);

int main (int argc, char *argv[])
{
    foo("Hahahahaha! Coool !");
    foo(32);
    
    return 0;
}
