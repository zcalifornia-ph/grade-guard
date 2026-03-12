#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

static int expect_true(int condition, const char* message)
{
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        return 0;
    }

    return 1;
}

int main(void)
{
    int values[] = {10, 20, 30, 40, 50, 60};
    const char* labels[] = {"quiz", "exam"};
    Vector* ints;
    Vector* strings;
    int* removed_int;
    const char** removed_label;
    size_t i;

    if (!expect_true(vector_create(0) == NULL, "vector_create should reject zero-sized elements")) {
        return 1;
    }

    ints = vector_create(sizeof(int));
    if (!expect_true(ints != NULL, "vector_create should allocate an int vector")) {
        return 1;
    }

    if (!expect_true(vector_size(ints) == 0, "new vector should start empty")) {
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(vector_at(ints, 0) == NULL, "vector_at should reject empty access")) {
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(vector_remove(ints, 0) == NULL, "vector_remove should reject empty access")) {
        vector_destroy(ints);
        return 1;
    }

    vector_pop_back(ints);
    if (!expect_true(vector_size(ints) == 0, "vector_pop_back on empty vector should be a no-op")) {
        vector_destroy(ints);
        return 1;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        vector_push_back(ints, &values[i]);
    }

    if (!expect_true(vector_size(ints) == 6, "push_back should append every value")) {
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(ints->capacity == 8, "vector should grow from 4 to 8 elements")) {
        vector_destroy(ints);
        return 1;
    }

    for (i = 0; i < vector_size(ints); i++) {
        int* value = (int*)vector_at(ints, i);

        if (!expect_true(value != NULL, "vector_at should return a valid element pointer")) {
            vector_destroy(ints);
            return 1;
        }

        if (!expect_true(*value == values[i], "vector_at should preserve pushed values")) {
            vector_destroy(ints);
            return 1;
        }
    }

    removed_int = (int*)vector_remove(ints, 2);
    if (!expect_true(removed_int != NULL, "vector_remove should return a removed element copy")) {
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(*removed_int == 30, "vector_remove should return the removed value")) {
        free(removed_int);
        vector_destroy(ints);
        return 1;
    }

    free(removed_int);

    if (!expect_true(vector_size(ints) == 5, "vector_remove should reduce the size")) {
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(*(int*)vector_at(ints, 2) == 40, "vector_remove should shift trailing elements")) {
        vector_destroy(ints);
        return 1;
    }

    vector_resize(ints, 4);
    if (!expect_true(ints->capacity == 8, "resize below current size should be ignored")) {
        vector_destroy(ints);
        return 1;
    }

    vector_resize(ints, 6);
    if (!expect_true(ints->capacity == 6, "resize should allow shrinking to current size or larger")) {
        vector_destroy(ints);
        return 1;
    }

    vector_resize(ints, 12);
    if (!expect_true(ints->capacity == 12, "resize should allow growth")) {
        vector_destroy(ints);
        return 1;
    }

    vector_pop_back(ints);
    if (!expect_true(vector_size(ints) == 4, "pop_back should reduce the size")) {
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(*(int*)vector_at(ints, 3) == 50, "pop_back should remove the last element")) {
        vector_destroy(ints);
        return 1;
    }

    strings = vector_create(sizeof(const char*));
    if (!expect_true(strings != NULL, "vector_create should allocate a pointer vector")) {
        vector_destroy(ints);
        return 1;
    }

    vector_push_back(strings, &labels[0]);
    vector_push_back(strings, &labels[1]);

    if (!expect_true(strcmp(*(const char**)vector_at(strings, 0), "quiz") == 0, "vector should store pointer values by value")) {
        vector_destroy(strings);
        vector_destroy(ints);
        return 1;
    }

    removed_label = (const char**)vector_remove(strings, 0);
    if (!expect_true(removed_label != NULL, "pointer-vector remove should return a copied pointer slot")) {
        vector_destroy(strings);
        vector_destroy(ints);
        return 1;
    }

    if (!expect_true(strcmp(*removed_label, "quiz") == 0, "removed pointer copy should preserve the pointed-to string")) {
        free(removed_label);
        vector_destroy(strings);
        vector_destroy(ints);
        return 1;
    }

    free(removed_label);
    vector_destroy(strings);
    vector_destroy(ints);

    printf("vector tests passed\n");
    return 0;
}
