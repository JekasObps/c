#include "binary_search.h"

typedef char (*ord_func) (int, int);

static int* binary_search_impl(int arr[], size_t n, int value, ord_func ord);
static char asc_order(int a, int b);
static char desc_order(int a, int b);

int* binary_search_asc(int arr[], size_t n, int value)
{
    return binary_search_impl(arr, n, value, asc_order);
}

int* binary_search_desc(int arr[], size_t n, int value)
{
    return binary_search_impl(arr, n, value, desc_order);
}

static int* binary_search_impl(int arr[], size_t n, int value, ord_func ord)
{
    int *left = arr, *right = arr+n-1, *mid = 0;
    
    while (left <= right)
    {
        mid = left + (right-left)/2;
        
        if (*mid == value)
        {
            return mid;
        }
        
        if (ord(value, *mid))
        {
            right = mid - 1;
        } 
        else
        {
            left = mid + 1;
        }
    }

    return 0; /* null */
}

static char asc_order(int a, int b)
{
    return a < b;
}

static char desc_order(int a, int b)
{
    return a > b;
}
