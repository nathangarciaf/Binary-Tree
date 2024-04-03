#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

//CORRIGIR REMOVE

KeyValPair *key_val_pair_construct(void *key, void *val){
    KeyValPair *kvp = (KeyValPair*)calloc(1,sizeof(KeyValPair));
    kvp->key = key;
    kvp->value = val;
    return kvp;
}

void key_val_pair_destroy(KeyValPair *kvp,KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn){
    key_destroy_fn(kvp->key);
    val_destroy_fn(kvp->value);
    free(kvp);
}

Node *node_construct(void *key, void *value, Node *left, Node *right){
    Node *node = (Node*)calloc(1,sizeof(Node));
    node->kvp = key_val_pair_construct(key,value);
    node->left = left;
    node->right = right;
    return node;
}

Node *_node_get_min(Node *node){
    if(node->left)
        return _node_get_min(node->left);
    return node;
}

Node *_node_get_max(Node *node){
    if(node->right)
        return _node_get_max(node->right);
    return node;
}

void node_destroy(Node *node,KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn){
    key_val_pair_destroy(node->kvp,key_destroy_fn,val_destroy_fn);
    free(node);
}

BinaryTree *binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn){
    BinaryTree *bt = (BinaryTree*)calloc(1,sizeof(BinaryTree));
    bt->size = 0;
    bt->cmp_fn = cmp_fn;
    bt->val_destroy_fn = val_destroy_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->root = NULL;
    return bt;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value){
    if(!bt->root){
        bt->root = node_construct(key,value,NULL,NULL);
        //printf("NÓ INSERIDO: %p\n",bt->root);
    }
    else{
        Node *root = bt->root;
        while(root){
            //DIREITA
            if(bt->cmp_fn(root->kvp->key,key) < 0){
                if(!root->right){
                    Node *node = node_construct(key,value,NULL,NULL);
                    root->right = node;
                    //printf("NÓ INSERIDO: %p\n",node);
                    break;
                }
                root = root->right;
            }
            else if(bt->cmp_fn(root->kvp->key,key) > 0){
                if(!root->left){
                    Node *node = node_construct(key,value,NULL,NULL);
                    root->left = node;
                    //printf("NÓ INSERIDO: %p\n",node);
                    break;
                }
                root = root->left;
            }
            else{
                bt->key_destroy_fn(key);
                bt->val_destroy_fn(root->kvp->value);
                root->kvp->value = value;
                break;
            }
        }
    }
    bt->size++;
}

int binary_tree_empty(BinaryTree *bt){
    if(!bt->root)
        return 1;
    return 0;
}

void binary_tree_remove(BinaryTree *bt, void *key){
    Node *node = bt->root;
    Node *parent = NULL;
    if(!bt->cmp_fn(node->kvp->key,key)){
        if(node->right != NULL){
            if(node->left == NULL){
                bt->root = node->right;
            }
            else{
                //printf("A RAIZ POSSUI ITEM A DIREITA\n");
                Node *right_root = node->right;
                if(!right_root->left){
                    right_root->left = bt->root->left;
                    bt->root = right_root;
                    //printf("O ITEM A DIREITA DA RAIZ NAO POSSUI ITEM MENOR QUE ELE\n");
                }
                else{
                    KeyValPair *kvp = _subtree_pop_min(right_root);
                    Node *new_node = node_construct(kvp->key,kvp->value,node->left,node->right);
                    free(kvp);
                    bt->root = new_node;
                }     
            }               
        }
        else if(node->left != NULL){
            //printf("A RAIZ SÓ TEM NÓ A ESQUERDA\n");
            bt->root = node->left;
        }
        else{
            //printf("EXISTE APENAS A RAIZ\n");
            bt->root = NULL;
        }
        node_destroy(node,bt->key_destroy_fn,bt->val_destroy_fn);
        return;
    }

    while(node){
        if(bt->cmp_fn(node->kvp->key,key) < 0){
            //printf("O ITEM QUE QUEREMOS POSSUI UM ITEM MAIOR QUE ELE\n");
            parent = node;
            node = node->right;
        }
        else if(bt->cmp_fn(node->kvp->key,key) > 0){
            //printf("O ITEM QUE QUEREMOS POSSUI UM ITEM MENOR QUE ELE\n");
            parent = node;
            node = node->left;
        }
        else{
            if(node->right != NULL){
                if(!node->left){
                    if(bt->cmp_fn(parent->kvp->key,key) < 0){
                        parent->right = node->right;
                    }
                    //ESQUERDA
                    else{
                        parent->left = node->right;
                    }
                }
                else{
                    Node *right_node = node->right;
                    if(!right_node->left){
                        if(bt->cmp_fn(parent->kvp->key,node->kvp->key) < 0){
                            right_node->left = node->left;
                            parent->right = right_node;
                        }
                        else{
                            right_node->left = node->left;
                            parent->left = right_node;
                        }
                    }
                    else{
                        KeyValPair *kvp = _subtree_pop_min(right_node);
                        Node *new_node = node_construct(kvp->key,kvp->value,node->left,node->right);
                        free(kvp);
                        if(bt->cmp_fn(parent->kvp->key,right_node->kvp->key) < 0){
                            parent->right = new_node;
                        }
                        else{
                            parent->left = new_node;
                        }
                    }
                    
                }
            }
            else if(node->left){
                //DIREITA
                if(bt->cmp_fn(parent->kvp->key,key) < 0){
                    parent->right = node->left;
                }
                //ESQUERDA
                else{
                    parent->left = node->left;
                }
            }
            else{
                if(bt->cmp_fn(parent->kvp->key,key) < 0){
                    parent->right = NULL;
                }
                //ESQUERDA
                else{
                    parent->left = NULL;
                }
            }
            node_destroy(node,bt->key_destroy_fn,bt->val_destroy_fn);
            break;
        }
    }
}

KeyValPair *binary_tree_min(BinaryTree *bt){
    Node *min = bt->root;
    while(min->left){
        min = min->left;
    }
    return min->kvp;
}

KeyValPair *binary_tree_max(BinaryTree *bt){
    Node *max = bt->root;
    while(max->right){
        max = max->right;
    }
    return max->kvp;
}

KeyValPair *binary_tree_pop_min(BinaryTree *bt){
    //VERIFICAR SE A ARVORE NAO ESTA VAZIA
    if(!bt->root){
        return NULL;
    }
    Node *node = bt->root;
    //VERIFICAR SE O NO DA ARVORE JA NAO EH O MINIMO
    if(!node->left){
        KeyValPair *kvp = node->kvp;
        if(!node->right){
            bt->root = NULL;
        }
        else{
            bt->root = node->right;
        }
        free(node);
        return kvp;
    }
    return _subtree_pop_min(bt->root);
}

KeyValPair *_subtree_pop_min(Node *subroot){
    Node *parent = NULL;
    Node *node = subroot;
    //PROCURA UM ITEM A ESQUERDA DO NO ATUAL ATE ACABAR
    while(node->left){
        parent = node;
        node = node->left;
    }
    KeyValPair *kvp = node->kvp;
    // VERIFICA SE O ITEM ATUAL (MINIMO) POSSUI ITEM A DIREITA
    // CASO POSSUA, O PAI DO ITEM MIN RECEBE ESTE ITEM A DIREITA DO ITEM MIN
    if(node->right){
        parent->left = node->right;
    }
    else{
        parent->left = NULL;
    }
    free(node);
    return kvp;
}

KeyValPair *binary_tree_pop_max(BinaryTree *bt){
    //VERIFICAR SE A ARVORE NAO ESTA VAZIA
    if(!bt->root){
        return NULL;
    }
    
    Node *node = bt->root;
    //VERIFICAR SE O NO DA ARVORE JA NAO EH O MAXIMO
    if(!node->right){
        KeyValPair *kvp = node->kvp;
        if(!node->left){
            bt->root = NULL;
        }
        else{
            bt->root = node->left;
        }     
        free(node);
        return kvp;
    }
    return _subtree_pop_max(bt->root);
}

KeyValPair *_subtree_pop_max(Node *subroot){
    //PROCURA UM ITEM A DIREITA DO NO ATUAL ATE ACABAR
    Node *parent = NULL;
    Node *node = subroot;
    while(node->right){
        parent = node;
        node = node->right;
    }
    KeyValPair *kvp = node->kvp;
    // VERIFICA SE O ITEM ATUAL (MAXIMO) POSSUI ITEM A ESQUERDA
    // CASO POSSUA, O PAI DO ITEM MAX RECEBE ESTE ITEM A ESQUERD DO ITEM MAX
    if(node->left){
        parent->right = node->left;
    }
    else{
        parent->right = NULL;
    }
    free(node);
    return kvp;
}

void *binary_tree_get(BinaryTree *bt, void *key){
    Node *root = bt->root;
    void *val = NULL;
    while(root){
        if(bt->cmp_fn(root->kvp->key,key) < 0){
            root = root->right;
        }
        else if(bt->cmp_fn(root->kvp->key,key) > 0){
            root = root->left;
        }
        else{
            val = root->kvp->value;
            break;
        }
    }
    return val;
}

void _node_destroy_recursive(Node *root,KeyDestroyFn key_destroy_fn,ValDestroyFn val_destroy_fn){
    if(root){
        _node_destroy_recursive(root->left,key_destroy_fn,val_destroy_fn);
        _node_destroy_recursive(root->right,key_destroy_fn,val_destroy_fn);
        node_destroy(root,key_destroy_fn,val_destroy_fn);
    }
    return;
}

void binary_tree_destroy(BinaryTree *bt){
    Node *root = bt->root;
    _node_destroy_recursive(root,bt->key_destroy_fn,bt->val_destroy_fn);
    free(bt);
}

// a funcao abaixo pode ser util para debug, mas nao eh obrigatoria.
// void binary_tree_print(BinaryTree *bt);

Vector *binary_tree_inorder_traversal(BinaryTree *bt){
    Vector *stack = vector_construct();
    Vector *v = vector_construct();
    Node *node = bt->root;
    while(1){
        while(node){
            vector_push_back(stack,node);
            node = node->left;
        }
        if(stack->size){
            node = vector_pop_back(stack);
            vector_push_back(v,node->kvp);
            node = node->right;
        }
        else{
            break;
        }
    }
    vector_destroy(stack);
    return v;
}

Vector *binary_tree_preorder_traversal(BinaryTree *bt){
    Vector *stack = vector_construct();
    Vector *v = vector_construct();
    Node *node = bt->root;
    vector_push_back(stack,node);
    while(stack->size){
        node = vector_pop_back(stack);
        vector_push_back(v,node->kvp);
        if(node->right){
            vector_push_back(stack,node->right);
        }
        if(node->left){
            vector_push_back(stack,node->left);
        }
    }
    vector_destroy(stack);
    return v;
}

Vector *binary_tree_postorder_traversal(BinaryTree *bt){
    Vector *stack1 = vector_construct();
    Vector *stack2 = vector_construct();
    Vector *v = vector_construct();
    Node *node = bt->root;
    vector_push_back(stack1,node);
    while (stack1->size){
        node = vector_pop_back(stack1);
        if(node->left){
            vector_push_back(stack1,node->left);
        }
        if(node->right){
            vector_push_back(stack1,node->right);
        }
        vector_push_back(stack2,node);
    }
    while(stack2->size){
        node = vector_pop_back(stack2);
        vector_push_back(v,node->kvp);
    }
    vector_destroy(stack1);
    vector_destroy(stack2);
    return v;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt){
    Vector *queue = vector_construct();
    Vector *v = vector_construct();
    Node *node = bt->root;
    vector_push_back(queue,node);
    while(vector_size(queue)){
        Node *curr_node = vector_pop_front(queue);
        if(curr_node){
            vector_push_back(v,curr_node->kvp);
            vector_push_back(queue,curr_node->left);
            vector_push_back(queue,curr_node->right);
        }
    }
    vector_destroy(queue);
    return v;
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt){
    Vector *v = vector_construct();
    _node_inorder(bt->root,v);
    return v;
}

void _node_inorder(Node *node, Vector *v){
    if (!node) {
        return;
    }
    _node_inorder(node->left,v);
    v->data[v->size] = node->kvp;
    v->size++;
    _node_inorder(node->right,v);
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt){
    Vector *v = vector_construct();
    _node_preorder(bt->root,v);
    return v;
}

void _node_preorder(Node *node, Vector *v){
    if (!node) {
        return;
    }
    v->data[v->size] = node->kvp;
    v->size++;
    _node_preorder(node->left,v);
    _node_preorder(node->right,v);
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt){
    Vector *v = vector_construct();
    _node_postorder(bt->root,v);
    return v;
}

void _node_postorder(Node *node, Vector *v){
    if (!node) {
        return;
    }
    _node_postorder(node->left,v);
    _node_postorder(node->right,v);
    v->data[v->size] = node->kvp;
    v->size++;
}