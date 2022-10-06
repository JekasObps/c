#include "slist_iter.h"

bool slist_iter_equals(SListIterator iter, SListIterator other)
{
    return iter.node == other.node;
}

bool slist_iter_is_valid(SListIterator iter)
{
    return node_is_valid(iter.node);
}

void* slist_iter_data(SListIterator iter)
{
    return node_get_data_ptr(iter.node);
}

SListIterator slist_iter_next(SListIterator iter)
{
    return (SListIterator){node_get_next(iter.node)}; 
}

void slist_iter_remove_after(SListIterator iter)
{
    Node *next = node_get_next(iter.node);
    
    if (next)
    {
        Node *next_next = node_get_next(next);
        node_free(next);

        node_set_next(iter.node, next_next);
    }
}
