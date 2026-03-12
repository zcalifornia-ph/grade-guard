#include "persistence.h"

void save_student_profile_to_csv(const Student_Profile* profile, const char* path)
{
    (void)profile;
    (void)path;
}

Student_Profile load_student_profile_from_csv(const char* path)
{
    Student_Profile profile = {0};

    (void)path;
    return profile;
}

int file_exists(const char* filename)
{
    (void)filename;
    return 0;
}

char* append_size_to_csv(size_t size)
{
    (void)size;
    return NULL;
}
