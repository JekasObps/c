#ifndef __FORWARD_LIST_H__
#define __FORWARD_LIST_H__

#include <stddef.h>

typedef struct forward_list forward_list;
typedef void* fl_iter;

typedef enum fl_status
{
    SUCCESS = 0, 
    FAIL,
    EMPTY,
    NOT_EMPTY
} fl_status;

typedef enum fl_validity
{
    VALID = 0,
    INVALID
} fl_validity;

/* user provided callback signature, takes pointer to a data stored in nodes,
 * returns status (foreach will not continue if status is FAIL) */
typedef fl_status (*act_on_data) (void**);

/* allocate memory for list and initialize it to zero elements,
 * if allocation won't succeed null ptr will be returned */
forward_list* forward_list_create(void);

/* deallocs all connected nodes and frees control structure */
void forward_list_destroy(forward_list *list);

/* push data at a top */
fl_status forward_list_push(forward_list *list, void* data);

/* pops data at a top and frees node, status will indicate success of an operation.
 * node data will be lost, so use peek before remove if you about to use it after pop */
fl_status forward_list_pop(forward_list *list);

/* peek top node and return status, status indicates success or failure
 * data will be returned via output param `out` */
fl_status forward_list_peek(const forward_list *list, void **out);

/* get element at specific index `idx`,
 * returns FAIL if index out of bounds, success otherwise.
 * data will be returned via `data_out` param */
fl_status forward_list_get(const forward_list *list, size_t idx, void **data_out);

/* checks when ever element is empty or not. 
 * returns EMPTY if it does, NOT_EMPTY otherwise. */
fl_status forward_list_is_empty(const forward_list *list);

/* counts how much elements are currently inside O(N) */
size_t forward_list_length(const forward_list *list);

/* iterator api */ 
fl_iter forward_list_iterator(const forward_list *list);
fl_iter fl_iterator_next(const fl_iter iter);
void*   fl_iterator_value(const fl_iter iter);
fl_validity fl_iterator_validity(fl_iter iter);
fl_status fl_iterator_foreach(fl_iter iter, act_on_data act);

#endif /* __FORWARD_LIST_H__ */
