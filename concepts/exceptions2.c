#include <setjmp.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>

#define SIZE_OF_BYTE 8
#define BITS_OF(type) sizeof(type)*SIZE_OF_BYTE

#define ERROR_STACK_SIZE 16

static thread_local jmp_buf jbuf[ERROR_STACK_SIZE] = {0};
static thread_local size_t j = 0;

#define TRY do { switch(setjmp(jbuf[j++])) { case 0: while(1) {
#define CATCH(x) break; case x:
#define ENDTRY }} while(0)
#define THROW(x) longjmp(jbuf[--j], x)
#define FINALLY break; } default:
#define EXCEPTION1 (1)
#define EXCEPTION2 (2)
#define EXCEPTION3 (3)

// forward decl
void f2(void);
void f3(void);
void f4(void);

void f1(void)
{
    TRY {
        f2();
    }
    CATCH(EXCEPTION1) {
        printf("f1: except(1) caught\n");
    }
    FINALLY {
        printf("exit f1\n");
    }
    ENDTRY;
}

void f2(void)
{
    TRY { 
        printf("f2: throw except(1)\n");
        THROW(EXCEPTION1);
    }
    CATCH(EXCEPTION2)
    {
        printf("f2: except(2) cought\n");
    }
    FINALLY {
        printf("exit f2\n");
    }
}

void f3(void)
{

}

void f4(void)
{

}


int main(int argc, char *argv[])
{
    f1();
    return 0;
}
