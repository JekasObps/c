#include "buble_sort.h"


void swap(void *restrict a, void *restrict b, size_t sz)
{
    char tmp[sz];
    memcpy(tmp, a, sz);
    memcpy(a, b, sz);
    memcpy(b, tmp, sz);
}

void int_swap(int *restrict a, int *restrict b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void buble_sort(void* arr, size_t sz, size_t n, is_before p)
{
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n - 1 - i; ++j)
        {
            char * b = arr;
            if (! p(b+sz*j, b+sz*(j+1)))
            {
                swap(b+sz*j, b+sz*(j+1), sz);
            }
        }
    }
}

void int_buble_sort(int arr[], size_t n, is_before p)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n - 1 - i; ++j)
        {
            if (! p(&arr[j], &arr[j+1]))
                int_swap(&arr[j], &arr[j+1]);
        }
    }
}


