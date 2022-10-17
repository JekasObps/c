#include <stdio.h>

int foo(int i)
{
    void* labels[3] = {&&lable_1, &&lable_2, &&lable_3};
    goto *labels[i];

    lable_1: 
        return 1; 
    lable_2: 
        return 2;
    lable_3: 
        return 3;
}

int main (int argc, char *argv[])
{
    
    printf("%d\n", foo(0));
    printf("%d\n", foo(1));
    printf("%d\n", foo(2));

    return 0;
}
