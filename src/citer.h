#ifndef _CITER_H_
#define _CITER_H_

#include <stdlib.h>

// Iterator type abstraction.
typedef struct {
    // A pointer to the root of the Iterator (allow late free and/or
    // iteration reset).
    void **root_pointer;
    // The length of the pointer queue.
    size_t queue_len;
    // Data pointer to the current element on the data queue.
    void *current;
    // The position of the current element on data queue.
    size_t current_pos;
    // If the iteration has reached the end.
    int is_done;
} CIterator;

CIterator *citerator_new(void);
void citerator_set(CIterator *, void *, void (*)(CIterator *, void *));
CIterator *citerator_new_from(void *, CIterator *(*)(void *));
int citerator_is_done(CIterator *);
void citerator_go_next(CIterator *);
void citerator_go_next_and_consume(CIterator *);
CIterator *citerator_go_next_or_free(CIterator *);
size_t citerator_get_index(CIterator *);
void *citerator_peek(CIterator *);
void citerator_reset(CIterator *);
void citerator_destroy(CIterator *);
void citerator_clear(CIterator *);

#endif /* _CITER_H_ */
