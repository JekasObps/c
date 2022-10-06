#include "algorithms.h"
#include "binary_search.h"

bool find_sum_of_two_asc(int arr[], size_t n, int sum)
{
    int *head = arr;
    int *tail = arr+n-1;

    while (head < tail)
    {
        int s = *head + *tail;
        
        if (s == sum)
            return true;
        
        if (s < sum)
            ++head;
        else
            --tail;
    }

    return false;
}

void find_ptrs_sum_of_two_asc(int arr[], size_t n, int sum, int* ptrs_out[2])
{
    int *head = arr;
    int *tail = arr+n-1;

    while (head < tail)
    {
        int s = *head + *tail;
        
        if (s == sum)
        {
            ptrs_out[0] = head;
            ptrs_out[1] = tail;
            return;
        }

        if (s < sum)
            ++head;
        else
            --tail;
    }

    ptrs_out[1] = ptrs_out[0] = 0; /* no result (nil) */
}

bool find_diff_of_two_asc(int arr[], size_t n, int diff)
{
    int* head;
    for (head = arr; head < arr+n-1; ++head)
    {
        int minuend = *head + diff;

        if( binary_search_asc(head, arr+n-head, minuend))
            return true;
    }

    return false;
}

void find_ptrs_diff_of_two_asc(int arr[], size_t n, int diff, int* ptrs_out[2])
{
    int *head;
    ptrs_out[1] = ptrs_out[0] = 0;
    
    for (head = arr; head < arr+n-1; ++head)
    {
        int minuend = *head + diff;
        int* subtrahend = binary_search_asc(head, arr+n-head, minuend);
        
        if (subtrahend)
        {
            ptrs_out[0] = head;
            ptrs_out[1] = subtrahend;
            return;
        }
    }    
}

bool find_sum_of_two_desc(int arr[], size_t n, int sum)
{
    int *head = arr;
    int *tail = arr+n-1;

    while (head < tail)
    {
        int s = *head + *tail;
        
        if (s == sum)
            return true;
        
        if (s > sum)
            ++head;
        else
            --tail;
    }

    return false;
}

void find_ptrs_sum_of_two_desc(int arr[], size_t n, int sum, int* ptrs_out[2])
{
    int *head = arr;
    int *tail = arr+n-1;

    while (head < tail)
    {
        int s = *head + *tail;
        
        if (s == sum)
        {
            ptrs_out[0] = head;
            ptrs_out[1] = tail;
            return;
        }

        if (s > sum)
            ++head;
        else
            --tail;
    }

    ptrs_out[1] = ptrs_out[0] = 0; /* no result (nil) */
}

bool find_diff_of_two_desc(int arr[], size_t n, int diff)
{
    int *head;
    for (head = arr; head < arr+n-1; ++head)
    {
        int subtrahend = *head - diff;

        if(binary_search_desc(head, arr+n-head, subtrahend))
            return true;
    }

    return false;
}

void find_ptrs_diff_of_two_desc(int arr[], size_t n, int diff, int* ptrs_out[2])
{
    int *head;
    ptrs_out[1] = ptrs_out[0] = 0;
    
    for (head = arr; head < arr+n-1; ++head)
    {
        int subtrahend = *head - diff;

        int* minuend = binary_search_desc(head, arr+n-head, subtrahend);
        
        if (minuend)
        {
            ptrs_out[0] = head;
            ptrs_out[1] = minuend;
            return;
        }
    }    
}
