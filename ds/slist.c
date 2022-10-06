#include "slist.h"

/* Base class 
 **/
struct SList
{
    Node *head;
    bool mm;
};

/* Derived Managed Memory class 
 **/
typedef struct SListMM
{
    SList base;
    size_t data_size;
} 
SListMM;

/* node factory method signature */
typedef Node* (*node_fm) (const SList *const list);

SList* slist_new(void)
{
    SList* list = (SList*) malloc(sizeof(SList));
    
    if (list)
    {
        list->head = 0; 
        list->mm = false;
    }

    return list;
}

SList* slist_new_mm(size_t data_size)
{
    SListMM *list = (SListMM*) malloc(sizeof(SListMM));
    assert(data_size);

    if (list)
    {
        list->base.head = 0;
        list->base.mm = true;
        list->data_size = data_size;
    }
    
    return (SList*)list;
}

void slist_free(SList *const list)
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

SListIterator slist_begin(SList *const list)
{
    assert(list);
    return (SListIterator){list->head};
}

/* forward decl, will be used by next func */
static Node* create_node(SList *const list); 

SListIterator slist_push_front(SList *const list)
{
    assert(list);
    Node *node = create_node(list);
    
    if (node)
    {
        node_set_next(node, list->head);
        list->head = node;
    }
        
    return (SListIterator){node};
}

/* forward decl, will be used by next function */
static Node* create_node_only(const SList *const list);
static Node* create_node_with_data(const SList *const list);

// private virtual method
static Node* create_node(SList *const list)
{
    static const node_fm f_tbl[2] = {
        create_node_only, 
        create_node_with_data
    };
    
    return f_tbl[list->mm](list);
}

static Node* create_node_only(const SList *const list)
{
    return node_new(FORWARD_LINKED);
}

static Node* create_node_with_data(const SList *const list)
{
    return node_new_with_data(FORWARD_LINKED, ((SListMM*) list)->data_size);
}

size_t slist_count(const SList *const list)
{
    size_t count = 0;
    assert(list);

    for (SListIterator it = slist_begin((SList *const)list); 
        slist_iter_is_valid(it); 
        it = slist_iter_next(it))
    {
        ++count;
    }

    return count;
}

int slist_foreach(SList *const list, user_apply f, void* param)
{
    for (SListIterator it = slist_begin(list); 
        slist_iter_is_valid(it); 
        it = slist_iter_next(it))
    {
        int status = f(slist_iter_data(it), param);
        
        if (SLIST_SUCCESS != status)
            return status;
    }

    return SLIST_SUCCESS;
}

