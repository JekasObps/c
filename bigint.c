
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define CHANK_SIZE 16

typedef size_t chank_t; /* 4 bits * 16 */
typedef struct bigint 
{
    chank_t *lsbeg;
    int size;
    char sign;
}
bigint_t;

/* allocate zero bigint */
bigint_t zero()
{
    chank_t* chank = calloc(1, sizeof(chank));
    return (bigint_t){.lsbeg = chank, .size = 1, .sign = 0};
}

/* deallocate bigint */
void free_bigint(bigint_t bi)
{
    free(bi.lsbeg);
}

bigint_t fromLong(long l)
{
    long n = l;
    int dcount = 0;

    while (n)
    {
        ++dcount;
        n /= 10;
    }
    
    int ccount = dcount / CHANK_SIZE + !!(dcount % CHANK_SIZE);
    
    bigint_t num;
    num.lsbeg = (chank_t*) calloc(ccount, sizeof(chank_t));
    num.size = ccount;
    num.sign = l >> 63;
    
    int c = 0;
    int d = 0;
    n = (l < 0)? -l : l;
    while (n)
    {
        num.lsbeg[c] |= (unsigned long)(n % 10) << (4 * d); 
        ++d; 
        n /= 10;
        
        if (d == CHANK_SIZE)
        {
            d = 0;
            ++c;
        }
    }
   
    return num;
}

void print_bigint(bigint_t bi)
{
    if ((int)bi.sign == -1)
        putc('-', stdout);
    
    int c = bi.size - 1;
    char prec_zeros = 0;

    for (; c >= 0; --c)
    {
        int d = CHANK_SIZE - 1;
        for (; d >= 0; --d)
        {
            long value = ((bi.lsbeg[c] >> (4*d)) & 0x0ful);
            if (!!value | prec_zeros)
            {
                prec_zeros = 1;
                putc(value + '0', stdout);
            }
        }
    }
    puts("\n");
}

int main(int argc, char *argv[])
{
    bigint_t big = fromLong(123456789);
    print_bigint(big);
    
    free_bigint(big);

    return EXIT_SUCCESS;
}
