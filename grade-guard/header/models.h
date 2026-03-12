#ifndef GRADE_GUARD_HEADER_MODELS_H
#define GRADE_GUARD_HEADER_MODELS_H

#include <stdbool.h>

#include "vector.h"

typedef struct Activities {
    char* activity_name;
    float score;
    float total_score;
} Activities;

typedef struct Course_Parameter {
    char* parameter_name;
    float weight;
    float percentage;
    Vector* activities;
} Course_Parameter;

typedef struct Course_Component {
    float weight;
    float grade;
    Vector* parameters;
} Course_Component;

typedef struct Course {
    char* name;
    float units;
    bool lab_flag;
    Course_Component lecture;
    Course_Component lab;
} Course;

typedef struct Student_Profile {
    char* first_name;
    char* middle_name;
    char* last_name;
    char* student_number;
    char* degree_program;
    Vector* courses;
    float predicted_gwa;
    float goal;
} Student_Profile;

/*
 * `*_reset()` releases fields owned by an embedded or stack-allocated struct
 * without freeing the struct itself.
 * `*_destroy()` additionally frees the wrapper pointer and is intended for
 * heap-owned instances such as copies returned by `vector_remove()`.
 */
bool activities_init(Activities* activity);
void activities_reset(Activities* activity);
void activities_destroy(Activities* activity);

bool course_parameter_init(Course_Parameter* parameter);
void course_parameter_reset(Course_Parameter* parameter);
void course_parameter_destroy(Course_Parameter* parameter);

bool course_component_init(Course_Component* component);
void course_component_reset(Course_Component* component);
void course_component_destroy(Course_Component* component);

bool course_init(Course* course);
void course_reset(Course* course);
void course_destroy(Course* course);

bool student_profile_init(Student_Profile* profile);
void student_profile_reset(Student_Profile* profile);
void student_profile_destroy(Student_Profile* profile);

#endif
