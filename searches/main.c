#include <stdio.h>
#include <stdint.h>


#include "binary_search.h"
#include "algorithms.h"

#define LEN(arr) ((sizeof(arr)/sizeof(*arr)))


void binary(void) 
{
    int n[] = {3, 12, 15, 20, 21, 29, 33, 45, 47, 55};    
    
    int *addr = binary_search_asc(n, LEN(n), 20);
    printf("%p -> %d\n", (void*)addr, (addr? *addr : INT32_MIN));
    
    addr = binary_search_asc(n, LEN(n), 22);
    printf("%p -> %d\n", (void*)addr, (addr? *addr : INT32_MIN));
}
    
void find_two_asc(void)
{
    int n[] = {5, 7, 9, 13, 15, 21};
    int* ptrs[2] = {0};

    bool res = find_sum_of_two_asc(n, LEN(n), 22);
    printf("sum of two(%d): %s\n", 22, BOOL_STR(res));
    
    find_ptrs_sum_of_two_asc(n, LEN(n), 22, ptrs);
    printf("sum of two(%d): %d %d \n", 22, *(ptrs[0]), *(ptrs[1]));
    
    res = find_diff_of_two_asc(n, LEN(n), 13);
    printf("diff of two(%d): %s\n", 13, BOOL_STR(res));

    find_ptrs_diff_of_two_asc(n, LEN(n), 12, ptrs);
    printf("diff of two(%d): %d %d \n", 12, *(ptrs[0]), *(ptrs[1]));
}

void find_two_desc(void)
{
    int n[] = {35, 15, 10, 5, -2, -7, -11};
    int* ptrs[2] = {0};

    bool res = find_sum_of_two_desc(n, LEN(n), 25);
    printf("sum of two(%d): %s\n", 25, BOOL_STR(res));
    
    find_ptrs_sum_of_two_desc(n, LEN(n), 25, ptrs);
    printf("sum of two(%d): %d %d \n", 25, *(ptrs[0]), *(ptrs[1]));
    
    res = find_diff_of_two_desc(n, LEN(n), 17);
    printf("diff of two(%d): %s\n", 17, BOOL_STR(res));

    find_ptrs_diff_of_two_desc(n, LEN(n), 17, ptrs);
    printf("diff of two(%d): %d %d \n", 17, *(ptrs[0]), *(ptrs[1]));
}

int main(int argc, char **argv)
{
    binary();
    find_two_asc();
    find_two_desc();
    return 0;
}
