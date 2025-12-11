#include "my_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fullfils a CIterator based on a given string. Fails if any param is
 * null pointer. */
void push_string_to_citerator(CIterator *citerator, char *str) {
    if (!str || !citerator)
        return;
    citerator_clear(citerator);
    size_t len = strlen(str);
    char **queue = (char **)malloc(len * sizeof(char *));
    if (!queue)
        return;
    citerator->queue_len = len;
    for (size_t i = 0; i < len; i++)
        queue[i] = &str[i];
    citerator->root_pointer = (void **)queue;
    citerator->current = citerator->root_pointer[0];
    citerator->is_done = 0;
}

/* Create a new CIterator function from the self string */
CIterator *new_citerator_from_string(char *self) {
    if (!self)
        return NULL;
    CIterator *citer = citerator_new();
    if (!citer)
        return NULL;
    push_string_to_citerator(citer, self);
    return citer;
}
