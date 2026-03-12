#ifndef GRADE_GUARD_HEADER_GRADE_CALC_H
#define GRADE_GUARD_HEADER_GRADE_CALC_H

#include "models.h"

#define GRADE_GUARD_GRADE_GOAL_COUNT 5

extern const float GRADE_GOALS[GRADE_GUARD_GRADE_GOAL_COUNT];

float percentage_to_gwa(float percentage);
float calculate_goal_percentage(float predicted_gwa, float goal_gwa);
float calculate_weighted_average(Student_Profile* profile);

#endif
