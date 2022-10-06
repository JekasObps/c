#ifndef __BUBLE_SORT_H__
#define __BUBLE_SORT_H__


#include <stddef.h>
#include <string.h>

typedef char (*is_before) (void *restrict a, void *restrict b);

void swap(void *a, void *b, size_t sz);
void int_swap(int *restrict a, int *restrict b);

void buble_sort(void *arr, size_t sz, size_t n, is_before p);
void int_buble_sort(int *arr, size_t n, is_before p);

#endif /* __BUBLE_SORT_H__ */
