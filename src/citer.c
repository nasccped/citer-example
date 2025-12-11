#include "citer.h"
#include <stdio.h>

void update_is_done(CIterator *);

/* Create a new empty CIterator. */
CIterator *citerator_new(void) {
    CIterator *citer = (CIterator *)malloc(sizeof(CIterator));
    if (!citer)
        return NULL;
    citer->root_pointer = NULL;
    citer->queue_len = 0;
    citer->current = NULL;
    citer->current_pos = 0;
    citer->is_done = 0;
    return citer;
}

/* Works similar to `citerator_new_from` function, but insert data
 * into an already allocated CIterator pointer. Does nothing if
 * `self` is null. Note that this function doesn't have any complex
 * task implemented, and it, in essence, calls the function pointer
 * that does the job, allowing generic behavior. */
void citerator_set(CIterator *self, void *data, void (*func)(CIterator *, void *)) {
    if (!self)
        return;
    func(self, data);
}

/* Creates a new CIterator from a generic data pointer + it's
 * destructuring function pointer. Note that the `func` parameter does
 * the conversion. */
CIterator *citerator_new_from(void *data, CIterator *(*func)(void *)) { return func(data); }

/* Returns if the iteration was reached the end. Note that this function returns 1 (true) if the
 * self pointer is now, preventing the user to try access on not allowed mem. */
int citerator_is_done(CIterator *self) { return self ? self->is_done : 1; }

/* Move the `current` pointer to the next item pointer on the
 * iteration queue without cleaning the struct inner fields when
 * iteration is done. */
void citerator_go_next(CIterator *self) {
    if (!self)
        return;
    else if (!citerator_is_done(self)) {
        self->current = self->root_pointer[++self->current_pos];
        update_is_done(self);
    } else {
        self->current_pos = 0;
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
    else if (!citerator_is_done(self)) {
        self->current = self->root_pointer[++self->current_pos];
        update_is_done(self);
    } else {
        if (self->root_pointer)
            free(self->root_pointer);
        self->root_pointer = NULL;
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
    else if (!citerator_is_done(self)) {
        self->current = self->root_pointer[++self->current_pos];
        update_is_done(self);
    } else {
        citerator_destroy(self);
        self = NULL;
    }
    return self;
}

/* Returns the position (0 based) of the current item on iteration. Note that this function returns
 * 0 if the self pointer is null. */
size_t citerator_get_index(CIterator *self) { return self ? self->current_pos : 0; }

/* Peeks the current item being pointed. */
void *citerator_peek(CIterator *self) { return self ? self->current : NULL; }

/* Resets the CIterator `current` field to the start of the iter
 * queue. Works only when `root_pointer` isn't NULL. */
void citerator_reset(CIterator *self) {
    if (!self)
        return;
    else if (self->root_pointer) {
        self->current = self->root_pointer[0];
        self->current_pos = 0;
        self->is_done = 0;
    }
}

/* Destroys the CIterator and it's inner data. */
void citerator_destroy(CIterator *self) {
    if (!self)
        return;
    citerator_clear(self);
    free(self);
}

/* Clear the CIterator fields without freeing the `self` pointer. */
void citerator_clear(CIterator *self) {
    if (!self)
        return;
    if (self->current)
        self->current = NULL;
    if (self->root_pointer) {
        free(self->root_pointer);
        self->root_pointer = NULL;
    }
    self->queue_len = 0;
    self->current_pos = 0;
    self->is_done = 1;
}

/* Private function that updates the `is_done` field based on other fields. */
void update_is_done(CIterator *self) {
    if (!self)
        return;
    else if (!self->is_done && self->current_pos >= self->queue_len)
        self->is_done = 1;
}
