#ifndef GRADE_GUARD_HEADER_PROFILE_CONTROLLER_H
#define GRADE_GUARD_HEADER_PROFILE_CONTROLLER_H

#include "models.h"

Student_Profile create_new_profile(void);
void input_courses(Student_Profile* profile);
void ui_show_profile_header(const Student_Profile* profile);
void ui_profile_login(Student_Profile* profile);

#endif
