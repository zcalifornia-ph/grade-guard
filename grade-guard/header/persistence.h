#ifndef GRADE_GUARD_HEADER_PERSISTENCE_H
#define GRADE_GUARD_HEADER_PERSISTENCE_H

#include "models.h"

void save_student_profile_to_csv(const Student_Profile* profile, const char* path);
Student_Profile load_student_profile_from_csv(const char* path);

int file_exists(const char* filename);
char* append_size_to_csv(size_t size);

#endif
