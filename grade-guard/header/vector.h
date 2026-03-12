#ifndef GRADE_GUARD_HEADER_VECTOR_H
#define GRADE_GUARD_HEADER_VECTOR_H

#include <stddef.h>

typedef struct Vector {
    void* data;
    size_t element_size;
    size_t capacity;
    size_t size;
} Vector;

Vector* vector_create(size_t element_size);
void vector_destroy(Vector* vec);

void vector_push_back(Vector* vec, const void* element);
void vector_pop_back(Vector* vec);
void* vector_remove(Vector* vec, size_t index);
void* vector_at(Vector* vec, size_t index);

size_t vector_size(const Vector* vec);
void vector_resize(Vector* vec, size_t new_capacity);

#endif
