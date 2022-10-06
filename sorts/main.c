#include <stdio.h>
#include "buble_sort.h"

#define LEN(arr) (sizeof(arr)/sizeof(*arr))

char int_is_before(int *restrict a, int *restrict b)
{
    return (*a < *b);
}

void print(int *arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
        printf("%d, ", arr[i]);
    putchar('\n');
}

int main(int argc, char **argv)
{
    int arr[] = { 9, 2, 4, 5, 1, 8 };
    print(arr, LEN(arr));
    puts("sort...\n");

    /* buble_sort(arr, sizeof(int), LEN(arr), (is_before)int_is_before); */
    int_buble_sort(arr, LEN(arr), (is_before)int_is_before);
    print(arr, LEN(arr));
}
