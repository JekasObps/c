#include <setjmp.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE_OF_BYTE 8
#define BITS_OF(type) sizeof(type)*SIZE_OF_BYTE

static jmp_buf buf;

typedef struct Comp
{
    short a:2;
    short b:2;
    short c:2;
    short d:BITS_OF(short)-2*3;
}
comp;

comp* comp_new(void)
{
    return (comp*)malloc(sizeof(comp));
}

void comp_free(comp *c)
{
    free(c);
}

void comp_init(comp *c)
{
    c->a = 1;
    c->b = -1;
    c->c = 0;
    c->d = 88;
}

void comp_destr(comp *c)
{
}

typedef struct Data 
{
    comp *a;
    comp *b;
} 
data;

data* data_new(void)
{
    return (data*)malloc(sizeof(data));
}

void data_free(data* d)
{
    free(d);
}

void data_init(data* d, short s)
{
    int err = 0;

    d->a = comp_new();
    if (0 == d->a) {err = 2; goto err_a;} 
    comp_init(d->a);

    /*d->b = comp_new();*/ d->b = 0;
    if (0 == d->b) {err = 3; goto err_b;}
    comp_init(d->b);

    d->b->a = ((comp*)&s)->a;
    
    return;
    
    /* on error*/
err_b:
    comp_destr(d->a);
    comp_free(d->a);
err_a:
    data_free(d);

    longjmp(buf, err); 
}

void data_destr(data* d)
{
    comp_destr(d->b);
    comp_free(d->b);
    d->b = 0;

    comp_destr(d->a);
    comp_free(d->a);
    d->a = 0;
}

int main(int argc, char *argv[])
{
    data *d;
    int err = setjmp(buf);

    switch(err)
    {
        case 0: /* entry */
            d = data_new();/* may fail */
            data_init(d, 3751);
            break;
        case 1: /* failed on data_new */
            printf("failed to allocate memory for data!");
            break;
        case 2: /* failed on data_init */
            printf("failed to allocate memory for data->a");
            break;
        case 3:
            printf("failed to allocate memory for data->b");
            break;
        default:
            printf("unknown error!");
    }
    
    if (0 == err)
    {
        data_destr(d);
        data_free(d);
    }

    return 0;
}
