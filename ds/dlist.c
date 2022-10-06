#include "dlist.h"

/* Base class 
 **/
struct DList
{
    Node *head;
    bool mm;
};

/* Derived Managed Memory class 
 **/
typedef struct DListMM
{
    DList base;
    size_t data_size;
} 
DListMM;

/* node factory method signature */
typedef Node* (*node_fm) (const DList *const list);

DList* dlist_new(void)
{
    DList* list = (DList*) malloc(sizeof(DList));
    
    if (list)
    {
        list->head = 0; 
        list->mm = false;
    }

    return list;
}

DList* dlist_new_mm(size_t data_size)
{
    DListMM *list = (DListMM*) malloc(sizeof(DListMM));
    assert(data_size);

    if (list)
    {
        list->base.head = 0;
        list->base.mm = true;
        list->data_size = data_size;
    }
    
    return (DList*)list;
}

void dlist_free(DList *const list)
{
    Node *node = list->head;
    assert(list);
    
    while(node)
    {
        Node *tmp = node_get_next(node);
        node_free(node);
        node = tmp;
    }
    
    free(list);
}

DListIterator dlist_begin(DList *const list)
{
    assert(list);
    return (DListIterator){list->head};
}

/* forward decl, will be used by next func */
static Node* create_node(DList *const list); 

DListIterator dlist_push_front(DList *const list)
{
    assert(list);
    Node *node = create_node(list);
    
    if (node)
    {
        node_set_next(node, list->head);
        node_set_prev(node, 0);
        if (list->head) node_set_prev(list->head, node);
        list->head = node;
    }
        
    return (DListIterator){node};
}

/* forward decl, will be used by next function */
static Node* create_node_only(const DList *const list);
static Node* create_node_with_data(const DList *const list);

// private virtual method
static Node* create_node(DList *const list)
{
    static const node_fm f_tbl[2] = {
        create_node_only, 
        create_node_with_data
    };
    
    return f_tbl[list->mm](list);
}

static Node* create_node_only(const DList *const list)
{
    return node_new(BACK_LINKED);
}

static Node* create_node_with_data(const DList *const list)
{
    return node_new_with_data(BACK_LINKED, ((DListMM*) list)->data_size);
}

int dlist_foreach(DList *const list, user_apply f, void* param)
{
    for (DListIterator it = dlist_begin(list); 
        dlist_iter_is_valid(it); 
        it = dlist_iter_next(it))
    {
        int status = f(dlist_iter_data(it), param);
        
        if (DLIST_SUCCESS != status)
            return status;
    }

    return DLIST_SUCCESS;
}

size_t dlist_count(const DList *const list)
{
    size_t count = 0;
    assert(list);

    for (DListIterator it = dlist_begin((DList *const)list); 
        dlist_iter_is_valid(it); 
        it = dlist_iter_next(it))
    {
        ++count;
    }

    return count;
}
