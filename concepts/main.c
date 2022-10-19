#include <stdio.h>


typedef struct S
{
    void (*fn)(int, int);
    int a, b; 
} S;


void foo(int a, int b)
{
    printf("%d, %d\n", a, b);
}

int main (int argc, char *argv[])
{ 

    printf("%f\n", 42.0);

    return 0;
}
