#ifndef GRADE_GUARD_UNIT_TESTS_TEST_FRAMEWORK_H
#define GRADE_GUARD_UNIT_TESTS_TEST_FRAMEWORK_H

#include <stddef.h>
#include <stdio.h>

typedef struct TestContext {
    int failures;
} TestContext;

#define TEST_EXPECT(context, condition, message)                                  \
    do {                                                                          \
        if (!(condition)) {                                                       \
            fprintf(stderr, "FAIL: %s (%s:%d)\n", message, __FILE__, __LINE__);   \
            (context)->failures++;                                                \
        }                                                                         \
    } while (0)

#define TEST_EXPECT_SIZE_EQ(context, expected, actual, message)                   \
    do {                                                                          \
        size_t expected_value__ = (expected);                                     \
        size_t actual_value__ = (actual);                                         \
        if (expected_value__ != actual_value__) {                                 \
            fprintf(                                                              \
                stderr,                                                           \
                "FAIL: %s (expected %zu, got %zu) (%s:%d)\n",                     \
                message,                                                          \
                expected_value__,                                                 \
                actual_value__,                                                   \
                __FILE__,                                                         \
                __LINE__                                                          \
            );                                                                    \
            (context)->failures++;                                                \
        }                                                                         \
    } while (0)

#endif
