#ifndef __SLIST_ITER_H__
#define __SLIST_ITER_H__

#include "node.h"
#include <stdbool.h>

typedef struct SListIterator
{
	Node* node;
}
SListIterator;

bool slist_iter_equals(SListIterator iter, SListIterator other);

bool slist_iter_is_valid(SListIterator iter);

void* slist_iter_data(SListIterator iter);

SListIterator slist_iter_next(SListIterator iter);

void slist_iter_remove_after(SListIterator iter);

#endif/*__SLIST_ITER_H__*/
