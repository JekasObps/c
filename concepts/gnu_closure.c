#include <stdio.h>

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
