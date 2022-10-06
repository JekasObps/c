
#include "forward_list.h"

#include <stdio.h>

/* function callback that will be used to test foreach */
static fl_status inc(void **data)
{
    ++*(int*)data;
    return SUCCESS;
}

int main(int argc, char **argv)
{
    forward_list* nums = forward_list_create();
   
    long data = 100;
    long peek;
    
    /* push data */
    forward_list_push(nums, (void*)data);
    
    /* peek data from top of the list */
    forward_list_peek(nums, (void**)&peek);
    printf("peek: %ld\n", peek);
    
    /* remove top */
    forward_list_pop(nums);

    /* fill list with values */
    for (long i = 0; i < 10l; ++i)
    {
        forward_list_push(nums, (void*)i);
    }
    
    /* apply inc on each element in a list */ 
    fl_iterator_foreach(forward_list_iterator(nums), inc);
    
    /* for iterator is valid print each element */
    for (fl_iter i = forward_list_iterator(nums); 
        VALID == fl_iterator_validity(i);
        i = fl_iterator_next(i))
    {
        printf("%ld ", (long)fl_iterator_value(i));
    }
    putchar('\n');
    
    /* count elements in a list */
    printf("nums.length = %lu\n", forward_list_length(nums));
    
    /* get specific element */
    forward_list_get(nums, 3, (void**)&peek);
    printf("nums[3] = %lu\n", peek);

    forward_list_destroy(nums);
    
    return 0;
}
