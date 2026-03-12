#ifndef GRADE_GUARD_HEADER_VECTOR_H
#define GRADE_GUARD_HEADER_VECTOR_H

#include <stddef.h>

typedef struct Vector {
    void* data;
    size_t element_size;
    size_t capacity;
    size_t size;
} Vector;

/*
 * Elements are stored by value as raw bytes.
 * If those bytes represent a pointer, the vector owns only the pointer-sized
 * slot, not the pointee.
 */
Vector* vector_create(size_t element_size);
void vector_destroy(Vector* vec);

/* Invalid arguments or allocation failure leave the vector unchanged. */
void vector_push_back(Vector* vec, const void* element);
void vector_pop_back(Vector* vec);

/*
 * Returns a heap-allocated copy of the removed element.
 * The caller owns the returned pointer and must free it.
 */
void* vector_remove(Vector* vec, size_t index);

/*
 * Returns a pointer into the vector's internal storage.
 * That pointer becomes invalid after mutation, resize, or destroy.
 */
void* vector_at(Vector* vec, size_t index);

size_t vector_size(const Vector* vec);

/*
 * Resize requests smaller than the current size, or zero-capacity requests,
 * are ignored.
 */
void vector_resize(Vector* vec, size_t new_capacity);

#endif
