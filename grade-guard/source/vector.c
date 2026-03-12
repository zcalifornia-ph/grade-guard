#include "vector.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY ((size_t)4)

static unsigned char* vector_bytes(Vector* vec)
{
    return (unsigned char*)vec->data;
}

Vector* vector_create(size_t element_size)
{
    Vector* vec;

    if (element_size == 0) {
        return NULL;
    }

    vec = (Vector*)malloc(sizeof(*vec));
    if (!vec) {
        return NULL;
    }

    vec->element_size = element_size;
    vec->capacity = VECTOR_INITIAL_CAPACITY;
    vec->size = 0;
    vec->data = malloc(vec->element_size * vec->capacity);

    if (!vec->data) {
        free(vec);
        return NULL;
    }

    return vec;
}

void vector_destroy(Vector* vec)
{
    if (!vec) {
        return;
    }

    free(vec->data);
    free(vec);
}

void vector_push_back(Vector* vec, const void* element)
{
    size_t new_capacity;

    if (!vec || !element || vec->element_size == 0) {
        return;
    }

    if (vec->size == vec->capacity) {
        new_capacity = vec->capacity == 0 ? VECTOR_INITIAL_CAPACITY : vec->capacity * 2;
        vector_resize(vec, new_capacity);

        if (vec->size == vec->capacity) {
            return;
        }
    }

    memcpy(
        vector_bytes(vec) + (vec->size * vec->element_size),
        element,
        vec->element_size
    );
    vec->size++;
}

void vector_pop_back(Vector* vec)
{
    if (!vec || vec->size == 0) {
        return;
    }

    vec->size--;
}

void* vector_remove(Vector* vec, size_t index)
{
    void* removed_element;

    if (!vec || index >= vec->size || vec->element_size == 0) {
        return NULL;
    }

    removed_element = malloc(vec->element_size);
    if (!removed_element) {
        return NULL;
    }

    memcpy(
        removed_element,
        vector_bytes(vec) + (index * vec->element_size),
        vec->element_size
    );

    if (index + 1 < vec->size) {
        memmove(
            vector_bytes(vec) + (index * vec->element_size),
            vector_bytes(vec) + ((index + 1) * vec->element_size),
            (vec->size - index - 1) * vec->element_size
        );
    }

    vec->size--;
    return removed_element;
}

void* vector_at(Vector* vec, size_t index)
{
    if (!vec || index >= vec->size) {
        return NULL;
    }

    return vector_bytes(vec) + (index * vec->element_size);
}

size_t vector_size(const Vector* vec)
{
    if (!vec) {
        return 0;
    }

    return vec->size;
}

void vector_resize(Vector* vec, size_t new_capacity)
{
    void* new_data;

    if (!vec || vec->element_size == 0) {
        return;
    }

    if (new_capacity == 0 || new_capacity < vec->size || new_capacity == vec->capacity) {
        return;
    }

    new_data = realloc(vec->data, vec->element_size * new_capacity);
    if (!new_data) {
        return;
    }

    vec->data = new_data;
    vec->capacity = new_capacity;
}
