#ifndef __SLIST_H__
#define __SLIST_H__

/**
 * Singly linked list
 *
 **/

#include "node.h"
#include "slist_iter.h"

typedef struct SList SList;

typedef enum SListStatus 
{
    SLIST_SUCCESS = 0
}
SListStatus;

/* function used by foreach */
typedef int (*user_apply)(void* data, void *param);

/* Create regular list */
SList* slist_new(void);

/* Creates list that manages memory allocation,
 * user need to specify size of an allocated data */
SList* slist_new_mm(size_t data_size);

/* Deallocates list and all of it's data, 
 * user is responsible for it's external data deallocation */
void slist_free(SList *const list);

/* Provide an Iterator pointing at front of the list */
SListIterator slist_begin(SList *const list);

/* Push element at front and provide iterator for it. 
 */
SListIterator slist_push_front(SList *const list);

/* O(N) counting of stored elements inside a given list
 */
size_t slist_count(const SList *const list);

/* Foreach element perform user defined apply function 
 */
int slist_foreach(SList *const list, user_apply f, void* param);


#endif/*__SLIST_H__*/
