#include <stdio.h>


#define THROW(e, ...) foo(e, (void*)(0,##__VA_ARGS__))

void foo(int n, void *p)
{
    printf("%p\n", p);
}

int main()
{
    int data;

    THROW(3);
    THROW(5, &data);
    
    return 0;
}
