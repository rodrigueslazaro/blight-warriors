#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

typedef struct {
    void *arr;
    size_t size;
    size_t capacity;
    size_t item_size;
} dynamic_array;

dynamic_array* dynamic_array_create(size_t item_size);
void dynamic_array_destroy(dynamic_array *d_array);
void dynamic_array_resize(dynamic_array *d_array, size_t new_capacity);
void dynamic_array_push_back(dynamic_array *d_array, const void *item);

#endif /* DYNAMIC_ARRAY_H */