#include "forward_list.h"

#include <stdlib.h>
#include <assert.h>

typedef struct node
{
    void *data;
    struct node *next;
} node;

struct forward_list
{
    node *head;
};

static node* node_create(void *data, node *next);
static void node_destroy(node *nod);

forward_list* forward_list_create(void)
{
    forward_list *list = (forward_list*) malloc(sizeof(*list));
    if (! list)
        return 0;
    
    list->head = 0;
    return list;
}

void forward_list_destroy(forward_list *list)
{
    assert(list);

    while (NOT_EMPTY == forward_list_is_empty(list))
    {
        forward_list_pop(list);
    }

    list->head = 0;
    free(list);
}

fl_status forward_list_push(forward_list *list, void *data)
{
    node *new;
    assert(list);
    
    new = node_create(data, list->head);
    
    if (!new)
        return FAIL;

    list->head = new;
    return SUCCESS;
}

fl_status forward_list_pop(forward_list *list)
{
    node *tmp;
    assert(list);

    tmp = list->head;
    list->head = tmp->next;

    node_destroy(tmp);

    return SUCCESS;
}

fl_status forward_list_peek(const forward_list *list, void **out)
{
    assert(list);
    
    if (EMPTY == forward_list_is_empty(list))
        return EMPTY;
    
    *out = list->head->data;
    return SUCCESS;
}


fl_status forward_list_get(const forward_list *list, size_t idx, void **data_out)
{
    size_t count = 0;
    fl_iter iter = forward_list_iterator(list);
    
    assert(list);
    
    while (VALID == fl_iterator_validity(iter) && count < idx)
    {
        iter = fl_iterator_next(iter);
        ++count;
    }

    if (VALID == fl_iterator_validity(iter))
    {
        *data_out = fl_iterator_value(iter);
        return SUCCESS;
    }

    return FAIL;
}

fl_status forward_list_is_empty(const forward_list *list)
{
    assert(list);

    return list->head
         ? NOT_EMPTY
         : EMPTY;
}

size_t forward_list_length(const forward_list *list)
{
    size_t length = 0;
    node *tmp;
    assert(list);
    
    tmp = list->head;

    while (tmp)
    {
        ++length;
        tmp = tmp->next;
    }

    return length;
}

fl_iter forward_list_iterator(const forward_list *list)
{
    assert(list);
    return (fl_iter)list->head;
}

fl_iter fl_iterator_next(const fl_iter iter)
{
    return ((node*)iter)->next;
}

void* fl_iterator_value(const fl_iter iter)
{
    return ((node*)iter)->data;
}

fl_validity fl_iterator_validity(const fl_iter iter)
{
    return iter ? VALID : INVALID;
}

fl_status fl_iterator_foreach(fl_iter iter, act_on_data act)
{
    while (VALID == fl_iterator_validity(iter))
    {
        if (FAIL == act(&((node*)iter)->data))
            return FAIL;

        iter = fl_iterator_next(iter);
    }

    return SUCCESS;
}


static node* node_create(void *data, node *next)
{
    node *nod = (node*) malloc(sizeof(node));
    
    if (!nod)
        return 0;
    
    nod->data = data;
    nod->next = next;

    return nod;
}

static void node_destroy(node *nod)
{
    nod->data = 0;
    nod->next = 0;
    free(nod);
}
