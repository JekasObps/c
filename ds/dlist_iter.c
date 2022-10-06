#include "dlist_iter.h"


bool dlist_iter_equals(DListIterator iter, DListIterator other)
{
    return iter.node == other.node;
}

bool dlist_iter_is_valid(DListIterator iter)
{
    return node_is_valid(iter.node);
}

void* dlist_iter_data(DListIterator iter)
{
    return node_get_data_ptr(iter.node);
}

DListIterator dlist_iter_next(DListIterator iter)
{
    return (DListIterator){node_get_next(iter.node)}; 
}

DListIterator dlist_iter_prev(DListIterator iter)
{
    return (DListIterator){node_get_prev(iter.node)};
}

void dlist_iter_remove(DListIterator iter)
{
    Node *prev = node_get_prev(iter.node);
    
    if (prev)
    {
        Node *next = node_get_next(iter.node); 
        node_free(iter.node);

        node_set_next(prev, next);
        node_set_prev(next, prev);
    }
}
