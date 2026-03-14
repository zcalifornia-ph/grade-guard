/*
Unit 5 baseline defect reproductions for the extracted grade engine and menu flow.
*/

#include <stdio.h>

#include "test_framework.h"
#include "../header/grade_calc.h"
#include "../header/models.h"
#include "../header/ui_console.h"

static float float_abs(float value)
{
    return value < 0.0f ? -value : value;
}

static int float_close(float expected, float actual)
{
    return float_abs(expected - actual) <= 0.0001f;
}

static int append_course(Student_Profile* profile, float units, int lab_flag, Course** out_course)
{
    Course course;
    size_t before_size;

    if (!profile || !profile->courses || !out_course) {
        return 0;
    }

    if (!course_init(&course)) {
        return 0;
    }

    course.units = units;
    course.lab_flag = lab_flag ? true : false;
    before_size = vector_size(profile->courses);
    vector_push_back(profile->courses, &course);
    if (vector_size(profile->courses) != before_size + 1) {
        course_reset(&course);
        return 0;
    }

    *out_course = (Course*)vector_at(profile->courses, before_size);
    return *out_course != NULL;
}

static int append_parameter(Course_Component* component, float weight, Course_Parameter** out_parameter)
{
    Course_Parameter parameter;
    size_t before_size;

    if (!component || !component->parameters || !out_parameter) {
        return 0;
    }

    if (!course_parameter_init(&parameter)) {
        return 0;
    }

    parameter.weight = weight;
    before_size = vector_size(component->parameters);
    vector_push_back(component->parameters, &parameter);
    if (vector_size(component->parameters) != before_size + 1) {
        course_parameter_reset(&parameter);
        return 0;
    }

    *out_parameter = (Course_Parameter*)vector_at(component->parameters, before_size);
    return *out_parameter != NULL;
}

static int append_activity(Course_Parameter* parameter, float score, float total_score)
{
    Activities activity;
    size_t before_size;

    if (!parameter || !parameter->activities) {
        return 0;
    }

    if (!activities_init(&activity)) {
        return 0;
    }

    activity.score = score;
    activity.total_score = total_score;
    before_size = vector_size(parameter->activities);
    vector_push_back(parameter->activities, &activity);
    if (vector_size(parameter->activities) != before_size + 1) {
        activities_reset(&activity);
        return 0;
    }

    return 1;
}

static void test_zero_score_activity_is_ignored(TestContext* context)
{
    Student_Profile profile = {0};
    Course* course;
    Course_Parameter* parameter;
    float observed_average;

    TEST_EXPECT(context, student_profile_init(&profile), "profile init should succeed for zero-score baseline");
    TEST_EXPECT(context, append_course(&profile, 3.0f, 0, &course), "course append should succeed for zero-score baseline");
    TEST_EXPECT(
        context,
        append_parameter(&course->lecture, 100.0f, &parameter),
        "parameter append should succeed for zero-score baseline"
    );
    TEST_EXPECT(context, append_activity(parameter, 0.0f, 100.0f), "zero-score activity append should succeed");
    TEST_EXPECT(context, append_activity(parameter, 100.0f, 100.0f), "non-zero activity append should succeed");

    observed_average = calculate_weighted_average(&profile);
    TEST_EXPECT(
        context,
        float_close(1.0f, observed_average),
        "baseline repro: zero-score activities are excluded from weighted-average totals"
    );

    student_profile_reset(&profile);
}

static void test_lab_course_scaling_explodes_course_percentage(TestContext* context)
{
    Student_Profile profile = {0};
    Course* course;
    Course_Parameter* lecture_parameter;
    Course_Parameter* lab_parameter;
    float observed_average;
    float observed_gwa;

    TEST_EXPECT(context, student_profile_init(&profile), "profile init should succeed for lab-scaling baseline");
    TEST_EXPECT(context, append_course(&profile, 3.0f, 1, &course), "course append should succeed for lab-scaling baseline");
    TEST_EXPECT(
        context,
        append_parameter(&course->lecture, 100.0f, &lecture_parameter),
        "lecture parameter append should succeed for lab-scaling baseline"
    );
    TEST_EXPECT(
        context,
        append_parameter(&course->lab, 100.0f, &lab_parameter),
        "lab parameter append should succeed for lab-scaling baseline"
    );
    TEST_EXPECT(context, append_activity(lecture_parameter, 50.0f, 100.0f), "lecture activity append should succeed");
    TEST_EXPECT(context, append_activity(lab_parameter, 50.0f, 100.0f), "lab activity append should succeed");

    observed_average = calculate_weighted_average(&profile);
    observed_gwa = percentage_to_gwa(observed_average);
    TEST_EXPECT(
        context,
        float_close(50.0f, observed_average),
        "baseline repro: lab-course percentage uses 50x scaling instead of a 50 percent share"
    );
    TEST_EXPECT(
        context,
        float_close(1.0f, observed_gwa),
        "baseline repro: impossible lab-course percentages clamp to a perfect 1.00 GWA"
    );

    student_profile_reset(&profile);
}

static void test_over_total_scores_are_accepted(TestContext* context)
{
    Student_Profile profile = {0};
    Course* course;
    Course_Parameter* parameter;
    float observed_average;
    float observed_gwa;

    TEST_EXPECT(context, student_profile_init(&profile), "profile init should succeed for over-total baseline");
    TEST_EXPECT(context, append_course(&profile, 3.0f, 0, &course), "course append should succeed for over-total baseline");
    TEST_EXPECT(
        context,
        append_parameter(&course->lecture, 100.0f, &parameter),
        "parameter append should succeed for over-total baseline"
    );
    TEST_EXPECT(context, append_activity(parameter, 120.0f, 100.0f), "over-total activity append should succeed");

    observed_average = calculate_weighted_average(&profile);
    observed_gwa = percentage_to_gwa(observed_average);
    TEST_EXPECT(
        context,
        float_close(1.2f, observed_average),
        "baseline repro: scores greater than the activity total inflate averages beyond 100 percent"
    );
    TEST_EXPECT(
        context,
        float_close(1.0f, observed_gwa),
        "baseline repro: inflated averages from over-total scores clamp to a perfect 1.00 GWA"
    );

    student_profile_reset(&profile);
}

static void test_zero_course_profiles_render_as_failing_gwa(TestContext* context)
{
    Student_Profile profile = {0};
    float observed_gwa;

    TEST_EXPECT(context, student_profile_init(&profile), "profile init should succeed for zero-course baseline");

    observed_gwa = percentage_to_gwa(calculate_weighted_average(&profile));
    TEST_EXPECT(
        context,
        float_close(5.0f, observed_gwa),
        "baseline repro: empty profiles map to a displayed 5.00 predicted GWA instead of a no-data state"
    );

    student_profile_reset(&profile);
}

static void test_escape_key_is_ignored_by_selection_handler(TestContext* context)
{
    int selected;
    int status;

    selected = 1;
    status = ui_selection_handler(UI_KEY_ESCAPE, &selected, UI_SELECTION_BOTH, 0, 4);
    TEST_EXPECT(
        context,
        status == 0,
        "baseline repro: escape does not propagate a cancel status from the selection handler"
    );
    TEST_EXPECT(
        context,
        selected == 1,
        "baseline repro: escape leaves the current selection unchanged instead of backing out of the menu"
    );
}

int main(void)
{
    TestContext context = {0};

    test_zero_score_activity_is_ignored(&context);
    test_lab_course_scaling_explodes_course_percentage(&context);
    test_over_total_scores_are_accepted(&context);
    test_zero_course_profiles_render_as_failing_gwa(&context);
    test_escape_key_is_ignored_by_selection_handler(&context);

    if (context.failures != 0) {
        return 1;
    }

    printf("unit 5 defect baseline reproductions passed\n");
    return 0;
}
