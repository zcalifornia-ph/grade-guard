#ifndef GRADE_GUARD_HEADER_PERSISTENCE_H
#define GRADE_GUARD_HEADER_PERSISTENCE_H

#include <stddef.h>

#include "models.h"

typedef enum PersistenceStatus {
    PERSISTENCE_STATUS_OK = 0,
    PERSISTENCE_STATUS_INVALID_ARGUMENT,
    PERSISTENCE_STATUS_IO_ERROR,
    PERSISTENCE_STATUS_MEMORY_ERROR,
    PERSISTENCE_STATUS_PARSE_ERROR,
    PERSISTENCE_STATUS_UNSUPPORTED_TEXT
} PersistenceStatus;

PersistenceStatus persistence_save_student_profile(const Student_Profile* profile, const char* path);
PersistenceStatus persistence_load_student_profile(const char* path, Student_Profile* profile);
char* persistence_build_profile_path(const char* directory, size_t index);
Vector* persistence_list_profile_file_paths(const char* directory);
void persistence_destroy_profile_file_paths(Vector* paths);

#endif
