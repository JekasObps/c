#include "node.h"

#include <stdint.h>
#include <stdio.h>

void printArr(int *arr, size_t size)
{
    putchar('[');
    for (size_t i = 0; i < size; i++)
    {
        printf("%d%s", arr[i], i != size - 1 ? ", " : "");
    }
    puts("]\n");
}


int main (int argc, char *argv[])
{
#if 0
    /* External memory allocation */
    Node *node = 0, 
         *tmp;
        
    for (int i = 0; i < 10; ++i)
    { 
        tmp = node_new();
        float *f = (float*) malloc(sizeof(float));
        /* 
         * check tmp != null and f != null 
         */
        
        *f = 0.12 * i; /* set data */
        
        node_set_data_ptr(tmp, f);
        node_set_next(tmp, node);
        node = tmp;
    }
    
    while (tmp)
    {
        printf("node(%f)\n", *(float*) node_get_data_ptr(tmp));
        free(node_get_data_ptr(tmp)); /* free user data */

        Node *n = tmp;
        tmp = node_get_next(tmp);
        node_free(n);
    }
#endif

/* compile with -DBACK_LINKS
 */
#if 0
    Node *curr = 0, *prev, *head;
    
    prev = head = node_new_with_data(sizeof(float));
    
    *(float*) node_get_data_ptr(head) = 0.1f;
    for (int i = 1; i < 10; ++i)
    {
        curr = node_new_with_data(sizeof(float));
        *(float*) node_get_data_ptr(curr) = i * 0.21f;
            
        node_set_next(prev, curr);
        node_set_prev(curr, prev);
        prev = curr;
    }
    
    curr = head;

    while (curr)
    {
        printf("node(%f)\n", *(float*) node_get_data_ptr(curr));
        prev = curr;
        curr = node_get_next(curr);
    }
    
    while (prev)
    {
        printf("node(%f)\n", *(float*) node_get_data_ptr(prev));
        curr = prev;
        prev = node_get_prev(prev);
        node_free(curr);
    }

#endif

#if 1
    Node *node = 0, *tmp;

    for (int i = 1; i < 10; ++i)
    {
        tmp = node_new_with_data(sizeof(int[5]));
        int *data = node_get_data_ptr(tmp);
        
        data[0] = 2<<i;
        data[1] = 3<<i;
        data[2] = 4<<i;
        data[3] = 5<<i;
        data[4] = 6<<i;
        node_set_next(tmp, node);
        node = tmp;
    }
    
    while (node)
    {
        printArr(node_get_data_ptr(node), 5);
        tmp = node;
        node = node_get_next(node);
        node_free(tmp);
    }
#endif

    return 0;
}
