#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DEREF(p, off) (*(p+off))



/**
 * foo - does nothing
 *
 * @param void
 *
 * @return 123
 */
int foo ()
{
	int v = 123;
	return v;
}
		
int main (int argc, char *argv[])
{
	int n = 0;
    const char * path = "/home/jekas/c/main.c";	
	foo();
    printf("hello world %d", 23);
	
	malloc(123);


	return 0;
}

