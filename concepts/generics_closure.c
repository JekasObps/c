/**
 *
 * Generics and closure in c 
 *
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


static thread_local int _0;
void _f(int b)
{
    int a = _0;
    printf("%d + %d = %d\n", a, b, a + b);
}

fn foo2(int a)
{
   _0 = a;
   return _f;
}

int thread_func(void * _)
{
    int a, b;
    
    for (int i = 0; i < 100; i++) {
        srand(clock());
        a = rand() % 20;
        b = rand() % 20;
        
        foo2(a)(b);
    }
    return thrd_success;
}

int main (int argc, char *argv[])
{
    foo("Hahahahaha! Coool !");
    foo(32);
    foo(2124);
    
    
    thrd_t thread1, thread2;
    thrd_create(&thread1, thread_func, 0);
    thrd_create(&thread2, thread_func, 0);
    
    thrd_join(thread1, 0);
    thrd_join(thread2, 0);

    return 0;
}
