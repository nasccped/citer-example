#include "posints.h"

/* Note: posint is a type alias for PositiveInts: an array of positive integers which the first
 * negative integer is used as safeguard value (since there's no way to track array length when the
 * pointer comes from outside the function). */

/* Push a sequence of positive integers into a CIterator. Note that this function requires the
 * posints array to contains an negative integer as safeguard. */
void push_posints_into_citerator(CIterator *citerator, int *posints) {
    if (!citerator || !posints)
        return;
    citerator_clear(citerator);
    size_t len;
    for (len = 0; posints[len] >= 0; len++)
        ;
    int **ints = (int **)malloc(len * sizeof(int *));
    if (!ints)
        return;
    citerator->queue_len = len;
    for (size_t i = 0; i < len; i++)
        ints[i] = &posints[i];
    citerator->root_pointer = (void **)ints;
    citerator->current = citerator->root_pointer[0];
    citerator->is_done = 0;
}

/* Creates a new CIterator from a posints. Don't forget the negative int safeguard. */
CIterator *new_citerator_from_posints(int *posints) {
    CIterator *citerator = citerator_new();
    if (citerator)
        push_posints_into_citerator(citerator, posints);
    return citerator;
}
