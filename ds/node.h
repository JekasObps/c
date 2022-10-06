#ifndef __NODE_H__
#define __NODE_H__

#include <assert.h>
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <stdbool.h> 

typedef struct Node Node;

typedef enum NodeType
{
    FORWARD_LINKED = 0,
    BACK_LINKED
} 
NodeType;

/* Allocates only node structure.
 */
Node* node_new(NodeType type);

/* Allocates node structure and contiguous user data region next to it.
 * @params:
 *   type      - determine node type to be allocated
 *   data_size - size of user data that will be part of the node.
 **/
Node* node_new_with_data(NodeType type, size_t data_size);

/* Pseudo node is an invalid node that acts like an anchor
 * which can help solve some complexity issues.
 **/
Node* node_pseudo_new(NodeType type);

/* Deallocate node, data allocated by user have to be freed before than */
void node_free(Node *node);

/* If node created by method node_new or node_new_with_data,
 * than it considered valid, otherwise the node is a pseudo node 
 * and doesn't contain any user data */
bool node_is_valid(const Node *const node);

/* Returns pointer to data stored inside node */
void* node_get_data_ptr(Node *const node);

/* Overwrite data pointer */
void node_set_data_ptr(Node *const node, const void *const data);

/* Returns pointer to a next node in chain */
Node* node_get_next(const Node *const node);

/* Overwrite pointer to next node */
void node_set_next(Node *const node, const Node *const next);

/* only for nodes of BACK_LINKED type, 
 * user responsibility */
Node* node_get_prev(const Node *const node);
void node_set_prev(Node *const node, const Node *const prev);

#endif/*__NODE_H__*/
