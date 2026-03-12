#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_framework.h"
#include "../header/models.h"

static char* duplicate_text(const char* value)
{
    size_t length;
    char* copy;

    if (!value) {
        return NULL;
    }

    length = strlen(value);
    copy = (char*)malloc(length + 1);
    if (!copy) {
        return NULL;
    }

    memcpy(copy, value, length + 1);
    return copy;
}

static int add_activity(Course_Parameter* parameter, const char* name, float score, float total_score)
{
    Activities activity;

    if (!parameter || !parameter->activities) {
        return 0;
    }

    if (!activities_init(&activity)) {
        return 0;
    }

    activity.activity_name = duplicate_text(name);
    if (!activity.activity_name) {
        return 0;
    }

    activity.score = score;
    activity.total_score = total_score;
    vector_push_back(parameter->activities, &activity);
    return vector_size(parameter->activities) > 0;
}

static int add_parameter(Course_Component* component, const char* name, float weight, const char* activity_name)
{
    Course_Parameter parameter;

    if (!component || !component->parameters) {
        return 0;
    }

    if (!course_parameter_init(&parameter)) {
        return 0;
    }

    parameter.parameter_name = duplicate_text(name);
    if (!parameter.parameter_name) {
        course_parameter_reset(&parameter);
        return 0;
    }

    parameter.weight = weight;
    parameter.percentage = 0.0f;

    if (!add_activity(&parameter, activity_name, 18.0f, 20.0f)) {
        course_parameter_reset(&parameter);
        return 0;
    }

    vector_push_back(component->parameters, &parameter);
    return vector_size(component->parameters) > 0;
}

static int add_course(Student_Profile* profile, const char* name, float units, int with_lab)
{
    Course course;

    if (!profile || !profile->courses) {
        return 0;
    }

    if (!course_init(&course)) {
        return 0;
    }

    course.name = duplicate_text(name);
    if (!course.name) {
        course_reset(&course);
        return 0;
    }

    course.units = units;
    course.lab_flag = with_lab != 0;

    if (!add_parameter(&course.lecture, "Lecture Requirements", 60.0f, "Lecture Quiz 1")) {
        course_reset(&course);
        return 0;
    }

    if (course.lab_flag && !add_parameter(&course.lab, "Laboratory", 40.0f, "Lab Exercise 1")) {
        course_reset(&course);
        return 0;
    }

    vector_push_back(profile->courses, &course);
    return vector_size(profile->courses) > 0;
}

int main(void)
{
    TestContext context = {0};
    Student_Profile profile;
    Course* first_course;
    Course_Parameter* lecture_parameter;
    Activities* removed_activity;
    Course_Parameter* removed_parameter;
    Course* removed_course;

    TEST_EXPECT(&context, student_profile_init(&profile), "student_profile_init should allocate the course vector");
    if (context.failures != 0) {
        return 1;
    }

    profile.first_name = duplicate_text("Alice");
    profile.middle_name = duplicate_text("B.");
    profile.last_name = duplicate_text("Student");
    profile.student_number = duplicate_text("2026-00001");
    profile.degree_program = duplicate_text("BSCS");
    profile.predicted_gwa = 1.75f;
    profile.goal = 1.45f;

    TEST_EXPECT(&context, profile.first_name && profile.middle_name && profile.last_name, "profile strings should be assignable");
    TEST_EXPECT(&context, add_course(&profile, "CMSC 22", 3.0f, 1), "first course should be addable with lecture and lab data");
    TEST_EXPECT(&context, add_course(&profile, "CMSC 56", 3.0f, 0), "second course should be addable without lab data");
    TEST_EXPECT_SIZE_EQ(&context, 2, vector_size(profile.courses), "profile should contain two courses after setup");

    first_course = (Course*)vector_at(profile.courses, 0);
    TEST_EXPECT(&context, first_course != NULL, "first course should be retrievable from the profile vector");
    TEST_EXPECT(&context, first_course->lecture.parameters != NULL, "course_init should allocate lecture parameter storage");
    TEST_EXPECT(&context, first_course->lab.parameters != NULL, "course_init should allocate lab parameter storage");

    lecture_parameter = (Course_Parameter*)vector_at(first_course->lecture.parameters, 0);
    TEST_EXPECT(&context, lecture_parameter != NULL, "lecture parameter should be retrievable");
    TEST_EXPECT_SIZE_EQ(&context, 1, vector_size(lecture_parameter->activities), "lecture parameter should start with one activity");

    removed_activity = (Activities*)vector_remove(lecture_parameter->activities, 0);
    TEST_EXPECT(&context, removed_activity != NULL, "vector_remove should return a heap copy for activities");
    activities_destroy(removed_activity);
    TEST_EXPECT_SIZE_EQ(&context, 0, vector_size(lecture_parameter->activities), "activity deletion should shrink the activity list");

    removed_parameter = (Course_Parameter*)vector_remove(first_course->lab.parameters, 0);
    TEST_EXPECT(&context, removed_parameter != NULL, "vector_remove should return a heap copy for parameters");
    course_parameter_destroy(removed_parameter);
    TEST_EXPECT_SIZE_EQ(&context, 0, vector_size(first_course->lab.parameters), "parameter deletion should shrink the lab parameter list");

    removed_course = (Course*)vector_remove(profile.courses, 1);
    TEST_EXPECT(&context, removed_course != NULL, "vector_remove should return a heap copy for courses");
    course_destroy(removed_course);
    TEST_EXPECT_SIZE_EQ(&context, 1, vector_size(profile.courses), "course deletion should shrink the course list");

    student_profile_reset(&profile);
    TEST_EXPECT(&context, profile.first_name == NULL, "student_profile_reset should clear owned strings");
    TEST_EXPECT(&context, profile.courses == NULL, "student_profile_reset should release the course vector");

    student_profile_reset(&profile);
    TEST_EXPECT(&context, profile.degree_program == NULL, "student_profile_reset should remain idempotent");

    if (context.failures != 0) {
        fprintf(stderr, "models lifecycle tests failed: %d\n", context.failures);
        return 1;
    }

    printf("models lifecycle tests passed\n");
    return 0;
}
