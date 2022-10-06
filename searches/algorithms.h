#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <stddef.h> /* size_t */

typedef char bool;
#define true 1
#define false 0
#define BOOL_STR(value) (value? "true" : "false")

/* function searches for two numbers in array that add up to a given sum.
 *  array have to be ascendingly sorted. returns 1 when found, otherwise 0.
 *  O(N)
 */
bool find_sum_of_two_asc(int arr[], size_t n, int sum);

/* searches for two numbers in ascendingly sorted array that differ by given amount. 
 * O(N*logN)
 */
bool find_diff_of_two_asc(int arr[], size_t n, int diff);

/* same sum search function, except it returns array of two pointers as a result (output param),
 * if not found both pointers will be set to zero.
 * O(N)
 */
void find_ptrs_sum_of_two_asc(int arr[], size_t n, int sum, int* ptrs_out[2]);

/* same diff search function, returns pair of pointers through ptrs_out.
 * O(N*logN)
 */
void find_ptrs_diff_of_two_asc(int arr[], size_t n, int diff, int* ptrs_out[2]);

/* same set of functions for sorted array in descending order */
bool find_sum_of_two_desc(int arr[], size_t n, int sum);
bool find_diff_of_two_desc(int arr[], size_t n, int diff);
void find_ptrs_sum_of_two_desc(int arr[], size_t n, int sum, int* ptrs_out[2]);
void find_ptrs_diff_of_two_desc(int arr[], size_t n, int diff, int* ptrs_out[2]);

#endif /* __ALGORITHMS_H__ */
