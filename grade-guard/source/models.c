#include "models.h"

#include <stdlib.h>

static void free_owned_string(char** value)
{
    if (!value) {
        return;
    }

    free(*value);
    *value = NULL;
}

bool activities_init(Activities* activity)
{
    if (!activity) {
        return false;
    }

    activity->activity_name = NULL;
    activity->score = 0.0f;
    activity->total_score = 0.0f;
    return true;
}

void activities_reset(Activities* activity)
{
    if (!activity) {
        return;
    }

    free_owned_string(&activity->activity_name);
    activity->score = 0.0f;
    activity->total_score = 0.0f;
}

void activities_destroy(Activities* activity)
{
    if (!activity) {
        return;
    }

    activities_reset(activity);
    free(activity);
}

bool course_parameter_init(Course_Parameter* parameter)
{
    if (!parameter) {
        return false;
    }

    parameter->parameter_name = NULL;
    parameter->weight = 0.0f;
    parameter->percentage = 0.0f;
    parameter->activities = vector_create(sizeof(Activities));
    return parameter->activities != NULL;
}

void course_parameter_reset(Course_Parameter* parameter)
{
    size_t i;

    if (!parameter) {
        return;
    }

    free_owned_string(&parameter->parameter_name);

    if (parameter->activities) {
        for (i = 0; i < vector_size(parameter->activities); i++) {
            activities_reset((Activities*)vector_at(parameter->activities, i));
        }

        vector_destroy(parameter->activities);
        parameter->activities = NULL;
    }

    parameter->weight = 0.0f;
    parameter->percentage = 0.0f;
}

void course_parameter_destroy(Course_Parameter* parameter)
{
    if (!parameter) {
        return;
    }

    course_parameter_reset(parameter);
    free(parameter);
}

bool course_component_init(Course_Component* component)
{
    if (!component) {
        return false;
    }

    component->weight = 0.0f;
    component->grade = 0.0f;
    component->parameters = vector_create(sizeof(Course_Parameter));
    return component->parameters != NULL;
}

void course_component_reset(Course_Component* component)
{
    size_t i;

    if (!component) {
        return;
    }

    if (component->parameters) {
        for (i = 0; i < vector_size(component->parameters); i++) {
            course_parameter_reset((Course_Parameter*)vector_at(component->parameters, i));
        }

        vector_destroy(component->parameters);
        component->parameters = NULL;
    }

    component->weight = 0.0f;
    component->grade = 0.0f;
}

void course_component_destroy(Course_Component* component)
{
    if (!component) {
        return;
    }

    course_component_reset(component);
    free(component);
}

bool course_init(Course* course)
{
    if (!course) {
        return false;
    }

    course->name = NULL;
    course->units = 0.0f;
    course->lab_flag = false;
    course->lecture.parameters = NULL;
    course->lab.parameters = NULL;

    if (!course_component_init(&course->lecture)) {
        return false;
    }

    if (!course_component_init(&course->lab)) {
        course_component_reset(&course->lecture);
        return false;
    }

    return true;
}

void course_reset(Course* course)
{
    if (!course) {
        return;
    }

    free_owned_string(&course->name);
    course_component_reset(&course->lecture);
    course_component_reset(&course->lab);
    course->units = 0.0f;
    course->lab_flag = false;
}

void course_destroy(Course* course)
{
    if (!course) {
        return;
    }

    course_reset(course);
    free(course);
}

bool student_profile_init(Student_Profile* profile)
{
    if (!profile) {
        return false;
    }

    profile->first_name = NULL;
    profile->middle_name = NULL;
    profile->last_name = NULL;
    profile->student_number = NULL;
    profile->degree_program = NULL;
    profile->courses = vector_create(sizeof(Course));
    profile->predicted_gwa = 0.0f;
    profile->goal = 0.0f;
    return profile->courses != NULL;
}

void student_profile_reset(Student_Profile* profile)
{
    size_t i;

    if (!profile) {
        return;
    }

    free_owned_string(&profile->first_name);
    free_owned_string(&profile->middle_name);
    free_owned_string(&profile->last_name);
    free_owned_string(&profile->student_number);
    free_owned_string(&profile->degree_program);

    if (profile->courses) {
        for (i = 0; i < vector_size(profile->courses); i++) {
            course_reset((Course*)vector_at(profile->courses, i));
        }

        vector_destroy(profile->courses);
        profile->courses = NULL;
    }

    profile->predicted_gwa = 0.0f;
    profile->goal = 0.0f;
}

void student_profile_destroy(Student_Profile* profile)
{
    if (!profile) {
        return;
    }

    student_profile_reset(profile);
    free(profile);
}
