#include "node.h"

#define PSEUDO_MARKER ((void*)0xDEADBEEF) 

struct Node
{
    void *data;
    struct Node *next;
};

typedef struct BackLinkedNode
{
    struct Node base;
    struct Node *prev;
} 
BLNode;

/* desition table function signature */
typedef Node* (*node_fm)(size_t);

static Node* def_node_new(size_t data_size);
static Node* bl_node_new(size_t data_size);

static const node_fm f_tbl[2] = {
    def_node_new,
    bl_node_new
};

Node *node_new(NodeType type)
{   
    return f_tbl[type](0);
}

Node* node_new_with_data(NodeType type, size_t data_size)
{
    return f_tbl[type](data_size);
}

Node* node_pseudo_new(NodeType type)
{
    Node *pseudo = node_new(type);
    pseudo->data = PSEUDO_MARKER;
    return pseudo;
}

static Node* def_node_new(size_t data_size)
{
    Node *new = (Node*) malloc(sizeof(Node) + data_size);
    
    if (new)
    {
        /* data region begins right after the node structure */
        new->data = (!!data_size) ? new + 1 : 0;
        new->next = 0;
    }

    return new;
}

static Node* bl_node_new(size_t data_size)
{
    BLNode* new = (BLNode*) malloc(sizeof(BLNode) + data_size);
    
    if (new)
    {
        new->base.data = (!!data_size) ? new + 1 : 0;
        new->base.next = 0;
        new->prev = 0;
    }

    return (Node*)new;
}

void node_free(Node *node)
{
    assert(node);
    free(node);
}

bool node_is_valid(const Node *const node)
{
    return node && node->data != PSEUDO_MARKER;
}

void* node_get_data_ptr(Node *const node)
{
    assert(node);
    return node->data;
}

void node_set_data_ptr(Node *const node, const void *const data)
{
    assert(node);
    node->data = (void*)data;
}

Node* node_get_next(const Node *const node)
{
    assert(node);
    return node->next;
}

void node_set_next(Node *const node, const Node *const next)
{
    assert(node);
    node->next = (Node*)next;
}

Node* node_get_prev(const Node *const node)
{
    assert(node);
    return ((BLNode*) node)->prev;
}

void node_set_prev(Node *const node, const Node *const prev)
{
    assert(node);
    ((BLNode*) node)->prev = (Node*)prev;
}
