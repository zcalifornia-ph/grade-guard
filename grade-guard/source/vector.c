#include "vector.h"

Vector* vector_create(size_t element_size)
{
    (void)element_size;
    return NULL;
}

void vector_destroy(Vector* vec)
{
    (void)vec;
}

void vector_push_back(Vector* vec, const void* element)
{
    (void)vec;
    (void)element;
}

void vector_pop_back(Vector* vec)
{
    (void)vec;
}

void* vector_remove(Vector* vec, size_t index)
{
    (void)vec;
    (void)index;
    return NULL;
}

void* vector_at(Vector* vec, size_t index)
{
    (void)vec;
    (void)index;
    return NULL;
}

size_t vector_size(const Vector* vec)
{
    (void)vec;
    return 0;
}

void vector_resize(Vector* vec, size_t new_capacity)
{
    (void)vec;
    (void)new_capacity;
}
