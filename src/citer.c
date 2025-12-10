#include "citer.h"
#include <stdio.h>

/* Create a new empty CIterator. */
CIterator *citerator_new(void) {
    CIterator *citer = (CIterator *)malloc(sizeof(CIterator));
    if (!citer)
        return NULL;
    citer->root_pointer = NULL;
    citer->queue_len = 0;
    citer->current = NULL;
    citer->current_pos = 0;
    return citer;
}

/* Works similar to `citerator_new_from` function, but insert data
 * into an already allocated CIterator pointer. Does nothing if
 * `self` is null. Note that this function doesn't have any complex
 * task implemented, and it, in essence, calls the function pointer
 * that does the job, allowing generic behavior. */
void citerator_set(CIterator *self, void *data,
                   void *(*func)(CIterator *, void *)) {
    if (!self)
        return;
    func(self, data);
}

/* Creates a new CIterator from a generic data pointer + it's
 * destructuring function pointer. Note that the `func` parameter does
 * the conversion. */
CIterator *citerator_new_from(void *data,
                              CIterator *(*func)(void *)) {
    return func(data);
}

/* Move the `current` pointer to the next item pointer on the
 * iteration queue without cleaning the struct inner fields when
 * iteration is done. */
void citerator_go_next(CIterator *self) {
    if (!self)
        return;
    if (self->current_pos++ < self->queue_len) {
        self->current = self->root_pointer[self->current_pos];
    } else {
        self->current = NULL;
    }
}

/* Move the `current` pointer to the next item pointer on the
 * iteration queue. When the iteration is done (`current_pos` >=
 * `queue_len`), the root_pointer is cleaned allowing the `self`
 * pointer to receive new items. */
void citerator_go_next_and_consume(CIterator *self) {
    if (!self)
        return;
    else if (self->current_pos++ < self->queue_len)
        self->current = self->root_pointer[self->current_pos];
    else {
        free(self->root_pointer);
        self->current = NULL;
        self->current_pos = 0;
        self->queue_len = 0;
    }
}

/* Move the `current` pointer to the next item pointer on the
 * iteration queue. When the iteration is done, the entire CIterator
 * is cleaned, useful within loop conditions! */
CIterator *citerator_go_next_or_free(CIterator *self) {
    if (!self)
        return NULL;
    else if (self->current_pos++ < self->queue_len)
        self->current = self->root_pointer[self->current_pos];
    else
        citerator_destroy(self);
    return self ? self : NULL;
}

/* Resets the CIterator `current` field to the start of the iter
 * queue. */
void citerator_reset(CIterator *self) {
    if (!self)
        return;
    self->current = self->root_pointer[0];
    self->current_pos = 0;
}

/* Destroys the CIterator and it's inner data. */
void citerator_destroy(CIterator *self) {
    if (!self)
        return;
    if (self->root_pointer)
        free(self->root_pointer);
    free(self);
}
