#include "b_tree.h"
#include <stdio.h>

size_t b_node_len(BTreeNode *);
BTreeNode *b_node_new(void *);
BTreeNode *b_node_insert(int (*)(void *, void *), BTreeNode *, BTreeNode *);
void b_node_destroy(void (*)(void *), BTreeNode *);
void send_b_node_to_queue(BTreeNode *, void **, size_t *);

/* Creates a new BinaryTree over a comparer function pointer + a free function pointer. */
BTree *b_tree_new(int (*comparer)(void *, void *), void (*free_func)(void *)) {
    BTree *tree = malloc(sizeof(BTree));
    if (!tree)
        return NULL;
    tree->comp = comparer;
    tree->free_func = free_func;
    tree->root = NULL;
    return tree;
}

/* Return the BinaryTree length. */
size_t b_tree_len(BTree *self) { return self ? b_node_len(self->root) : 0; }

/* Insert the new data onto the tree. */
void b_tree_insert(BTree *self, void *data) {
    if (!self || !data)
        return;
    BTreeNode *node = b_node_new(data);
    if (!node)
        return;
    self->root = b_node_insert(self->comp, self->root, node);
}

/* Destroy the entire tree + return a NULL pointer. */
BTree *b_tree_destroy(BTree *self) {
    if (!self)
        return NULL;
    b_node_destroy(self->free_func, self->root);
    free(self);
    return NULL;
}

/* Push the tree items into the provided CIterator pointer. */
void push_tree_into_citerator(CIterator *citer, BTree *tree) {
    if (!citer || !tree)
        return;
    citerator_clear(citer);
    size_t len = b_tree_len(tree);
    void **datas = (void **)malloc(sizeof(void *) * len);
    if (!datas)
        return;
    size_t ind = 0;
    send_b_node_to_queue(tree->root, datas, &ind);
    citer->queue_len = len;
    citer->root_pointer = datas;
    citer->current = citer->root_pointer[0];
    citer->is_done = 0;
}

/* Convert a BTree pointer into a CIterator pointer (doesn't free the tree). */
CIterator *new_citerator_from_b_tree(BTree *self) {
    if (!self)
        return NULL;
    CIterator *citer = citerator_new();
    if (citer)
        push_tree_into_citerator(citer, self);
    return citer;
}

/* Returns the BTreeNode length based on the current node + recursion. */
size_t b_node_len(BTreeNode *self) {
    return self ? 1 + b_node_len(self->left) + b_node_len(self->right) : 0;
}

/* Creates a new BTreeNode based on a given void pointer. */
BTreeNode *b_node_new(void *data) {
    if (!data)
        return NULL;
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (!node)
        return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Insert a new node in the current node pointer leafs based on the return value of the comp
 * function pointer. This function returns a BTreeNode pointer, so, it's intended to run as
 * recursion like function, where the incoming pointer is returned if the current node is NULL,
 * otherwise, the current leafs are update the self current is returned. */
BTreeNode *b_node_insert(int (*comp)(void *, void *), BTreeNode *current, BTreeNode *incoming) {
    if (!incoming)
        return current;
    else if (!current)
        return incoming;
    else if (comp(current->data, incoming->data) > 0)
        current->left = b_node_insert(comp, current->left, incoming);
    else
        current->right = b_node_insert(comp, current->right, incoming);
    return current;
}

/* A recursion like free function. */
void b_node_destroy(void (*free_func)(void *), BTreeNode *self) {
    if (!self)
        return;
    b_node_destroy(free_func, self->left);
    b_node_destroy(free_func, self->right);
    free_func(self->data);
    free(self);
}

/* Get the data from BTreeNode (recursively) and sets it into the data queue (void **). */
void send_b_node_to_queue(BTreeNode *self, void **queue, size_t *ind) {
    if (!self) {
        *ind -= 1;
        return;
    } else if (self->left) {
        send_b_node_to_queue(self->left, queue, ind);
        *ind += 1;
    }
    queue[*ind] = self->data;
    *ind += 1;
    // this is allowed since NULL pointer is checked within this function...
    send_b_node_to_queue(self->right, queue, ind);
}
