#ifndef __DLIST_ITER_H__
#define __DLIST_ITER_H__

#include "node.h"
#include <stdbool.h>

typedef struct DListIterator
{
	Node* node;
}
DListIterator;

bool dlist_iter_equals(DListIterator iter, DListIterator other);

bool dlist_iter_is_valid(DListIterator iter);

void* dlist_iter_data(DListIterator iter);

DListIterator dlist_iter_next(DListIterator iter);

DListIterator dlist_iter_prev(DListIterator iter);

void dlist_iter_remove(DListIterator iter);

#endif/*__DLIST_ITER_H__*/
