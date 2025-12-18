#ifndef _B_TREE_H_
#define _B_TREE_H_

#include "citer.h"
#include <stdlib.h>

/* Type abstraction for a BinaryTree node. */
typedef struct _BTreeNode {
    /* The actual data being hold. */
    void *data;
    /* The two leafs of a the current node. */
    struct _BTreeNode *left, *right;
} BTreeNode;

/* Type that represents a BinaryTree. */
typedef struct {
    /* Tree root. */
    BTreeNode *root;
    /* Comparer function (assert eq between valua $0 and $1). */
    int (*comp)(void *, void *);
    /* Function used the free the BinaryTree data (if necessary). */
    void (*free_func)(void *);
} BTree;

BTree *b_tree_new(int (*)(void *, void *), void (*)(void *));
size_t b_tree_len(BTree *);
void b_tree_insert(BTree *, void *);
BTree *b_tree_destroy(BTree *);
void push_tree_into_citerator(CIterator *, BTree *);
CIterator *new_citerator_from_b_tree(BTree *);

#endif
