
#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "vector.h"

typedef int (*CmpFn)(void *, void *);
typedef void (*KeyDestroyFn)(void *);
typedef void (*ValDestroyFn)(void *);

typedef struct
{
    void *key;
    void *value;
} KeyValPair;

typedef struct Node
{
    KeyValPair *kvp;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct
{
    Node *root;
    CmpFn cmp_fn;
    KeyDestroyFn key_destroy_fn;
    ValDestroyFn val_destroy_fn;
    int size;
} BinaryTree;

KeyValPair *key_val_pair_construct(void *key, void *val);
void key_val_pair_destroy(KeyValPair *kvp,KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn);

Node *node_construct(void *key, void *value, Node *left, Node *right);
Node *_node_get_min(Node *node);
Node *_node_get_max(Node *node);
void node_destroy(Node *node,KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn);
void _node_destroy_recursive(Node *root,KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn);

BinaryTree *binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn);
void binary_tree_add(BinaryTree *bt, void *key, void *value);
int binary_tree_empty(BinaryTree *bt);
void binary_tree_remove(BinaryTree *bt, void *key);
KeyValPair *binary_tree_min(BinaryTree *bt);
KeyValPair *binary_tree_max(BinaryTree *bt);
KeyValPair *binary_tree_pop_min(BinaryTree *bt);
KeyValPair *_subtree_pop_min(Node *subroot);
KeyValPair *binary_tree_pop_max(BinaryTree *bt);
KeyValPair *_subtree_pop_max(Node *subroot);
void *binary_tree_get(BinaryTree *bt, void *key);
void binary_tree_destroy(BinaryTree *bt);

// a funcao abaixo pode ser util para debug, mas nao eh obrigatoria.
// void binary_tree_print(BinaryTree *bt);

Vector *binary_tree_inorder_traversal(BinaryTree *bt);
Vector *binary_tree_preorder_traversal(BinaryTree *bt);
Vector *binary_tree_postorder_traversal(BinaryTree *bt);
Vector *binary_tree_levelorder_traversal(BinaryTree *bt);

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt);
void _node_inorder(Node *node, Vector *v);
Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt);
void _node_preorder(Node *node, Vector *v);
Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt);
void _node_postorder(Node *node, Vector *v);

#endif