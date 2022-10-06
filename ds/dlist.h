#ifndef __DLIST_H__
#define __DLIST_H__

/**
 * 
 * Doubly linked list
 *
 **/

#include "node.h"
#include "dlist_iter.h"

typedef struct DList DList;

typedef enum DListStatus 
{
    DLIST_SUCCESS = 0
}
DListStatus;

/* function used by foreach */
typedef int (*user_apply)(void* data, void *param);

/* Create regular list */
DList* dlist_new(void);

/* Creates list that manages memory allocation,
 * user need to specify size of an allocated data */
DList* dlist_new_mm(size_t data_size);

/* Deallocates list and all of it's data, 
 * user is responsible for it's external data deallocation */
void dlist_free(DList *const list);

/* Provide an Iterator pointing at front of the list */
DListIterator dlist_begin(DList *const list);

/* Push element at front and provide iterator for it. 
 */
DListIterator dlist_push_front(DList *const list);

/* O(N) counting of stored elements inside a given list
 */
size_t dlist_count(const DList *const list);

/* Foreach element perform user defined apply function 
 */
int dlist_foreach(DList *const list, user_apply f, void* param);

#endif/*__DLIST_H__*/
