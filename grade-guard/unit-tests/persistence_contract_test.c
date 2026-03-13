#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIRECTORY(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MAKE_DIRECTORY(path) mkdir(path, 0777)
#endif

#include "test_framework.h"
#include "../header/models.h"
#include "../header/persistence.h"

#define FLOAT_TOLERANCE (0.001f)

typedef struct ActivitySpec {
    const char* name;
    float score;
    float total_score;
} ActivitySpec;

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

static float absolute_difference(float left, float right)
{
    float difference = left - right;

    return difference < 0.0f ? -difference : difference;
}

static void expect_float_close(TestContext* context, float expected, float actual, const char* message)
{
    if (absolute_difference(expected, actual) > FLOAT_TOLERANCE) {
        fprintf(
            stderr,
            "FAIL: %s (expected %.3f, got %.3f) (%s:%d)\n",
            message,
            expected,
            actual,
            __FILE__,
            __LINE__
        );
        context->failures++;
    }
}

static void expect_string_equal(TestContext* context, const char* expected, const char* actual, const char* message)
{
    int matches = 0;

    if (!expected && !actual) {
        matches = 1;
    } else if (expected && actual && strcmp(expected, actual) == 0) {
        matches = 1;
    }

    TEST_EXPECT(context, matches, message);
}

static int push_element_checked(Vector* vector, const void* element)
{
    size_t previous_size;

    previous_size = vector_size(vector);
    vector_push_back(vector, element);
    return vector_size(vector) == previous_size + 1;
}

static int add_activity(Course_Parameter* parameter, const char* name, float score, float total_score)
{
    Activities activity;

    if (!parameter || !parameter->activities || !activities_init(&activity)) {
        return 0;
    }

    activity.activity_name = duplicate_text(name);
    if (!activity.activity_name) {
        return 0;
    }

    activity.score = score;
    activity.total_score = total_score;

    if (!push_element_checked(parameter->activities, &activity)) {
        activities_reset(&activity);
        return 0;
    }

    return 1;
}

static int add_parameter(
    Course_Component* component,
    const char* name,
    float weight,
    float percentage,
    const ActivitySpec* activities,
    size_t activity_count
)
{
    Course_Parameter parameter;
    size_t i;

    if (!component || !component->parameters || !course_parameter_init(&parameter)) {
        return 0;
    }

    parameter.parameter_name = duplicate_text(name);
    if (!parameter.parameter_name) {
        course_parameter_reset(&parameter);
        return 0;
    }

    parameter.weight = weight;
    parameter.percentage = percentage;

    for (i = 0; i < activity_count; i++) {
        if (!add_activity(&parameter, activities[i].name, activities[i].score, activities[i].total_score)) {
            course_parameter_reset(&parameter);
            return 0;
        }
    }

    if (!push_element_checked(component->parameters, &parameter)) {
        course_parameter_reset(&parameter);
        return 0;
    }

    return 1;
}

static int add_course(Student_Profile* profile, Course* course)
{
    if (!profile || !profile->courses || !course) {
        return 0;
    }

    return push_element_checked(profile->courses, course);
}

static int build_shape_lecture_only(Student_Profile* profile)
{
    Course course;
    const ActivitySpec quiz_activities[] = {
        {"Quiz 1", 18.00f, 20.00f},
        {"Quiz 2", 17.00f, 20.00f}
    };
    const ActivitySpec exam_activities[] = {
        {"Midterm Exam", 82.00f, 100.00f}
    };

    if (!student_profile_init(profile) || !course_init(&course)) {
        return 0;
    }

    profile->first_name = duplicate_text("Alice");
    profile->middle_name = duplicate_text("B");
    profile->last_name = duplicate_text("Student");
    profile->student_number = duplicate_text("2026-10001");
    profile->degree_program = duplicate_text("BSCS");
    profile->predicted_gwa = 1.75f;
    profile->goal = 1.20f;

    course.name = duplicate_text("CMSC 11");
    if (!profile->first_name || !profile->middle_name || !profile->last_name || !profile->student_number || !profile->degree_program || !course.name) {
        course_reset(&course);
        student_profile_reset(profile);
        return 0;
    }

    course.units = 3.00f;
    course.lab_flag = 0;

    if (!add_parameter(&course.lecture, "Quizzes", 40.00f, 0.00f, quiz_activities, 2)
        || !add_parameter(&course.lecture, "Exams", 60.00f, 0.00f, exam_activities, 1)
        || !add_course(profile, &course)) {
        course_reset(&course);
        student_profile_reset(profile);
        return 0;
    }

    return 1;
}

static int build_shape_lab(Student_Profile* profile)
{
    Course course;
    const ActivitySpec lecture_activities[] = {
        {"Lecture Quiz", 9.00f, 10.00f}
    };
    const ActivitySpec lab_activities[] = {
        {"Lab Exercise", 47.00f, 50.00f},
        {"Lab Practical", 44.00f, 50.00f}
    };

    if (!student_profile_init(profile) || !course_init(&course)) {
        return 0;
    }

    profile->first_name = duplicate_text("Bea");
    profile->middle_name = duplicate_text("");
    profile->last_name = duplicate_text("Lab");
    profile->student_number = duplicate_text("2026-10002");
    profile->degree_program = duplicate_text("BSCS");
    profile->predicted_gwa = 1.45f;
    profile->goal = 1.45f;

    course.name = duplicate_text("CMSC 127");
    if (!profile->first_name || !profile->middle_name || !profile->last_name || !profile->student_number || !profile->degree_program || !course.name) {
        course_reset(&course);
        student_profile_reset(profile);
        return 0;
    }

    course.units = 1.00f;
    course.lab_flag = 1;

    if (!add_parameter(&course.lecture, "Lecture", 50.00f, 0.00f, lecture_activities, 1)
        || !add_parameter(&course.lab, "Laboratory", 50.00f, 0.00f, lab_activities, 2)
        || !add_course(profile, &course)) {
        course_reset(&course);
        student_profile_reset(profile);
        return 0;
    }

    return 1;
}

static int build_shape_mixed(Student_Profile* profile)
{
    Course lecture_course;
    Course lab_course;
    const ActivitySpec lecture_only_activities[] = {
        {"Seatwork", 10.00f, 10.00f}
    };
    const ActivitySpec mixed_lecture_activities[] = {
        {"Problem Set 1", 18.00f, 20.00f},
        {"Problem Set 2", 20.00f, 20.00f}
    };
    const ActivitySpec mixed_lab_activities[] = {
        {"Machine Problem", 95.00f, 100.00f}
    };

    if (!student_profile_init(profile) || !course_init(&lecture_course) || !course_init(&lab_course)) {
        return 0;
    }

    profile->first_name = duplicate_text("Chris");
    profile->middle_name = duplicate_text("D");
    profile->last_name = duplicate_text("Mixer");
    profile->student_number = duplicate_text("2026-10003");
    profile->degree_program = duplicate_text("BS Computer Science");
    profile->predicted_gwa = 1.20f;
    profile->goal = 1.00f;

    lecture_course.name = duplicate_text("MATH 53");
    lab_course.name = duplicate_text("CMSC 22");
    if (!profile->first_name || !profile->middle_name || !profile->last_name || !profile->student_number || !profile->degree_program || !lecture_course.name || !lab_course.name) {
        course_reset(&lecture_course);
        course_reset(&lab_course);
        student_profile_reset(profile);
        return 0;
    }

    lecture_course.units = 5.00f;
    lecture_course.lab_flag = 0;
    lab_course.units = 3.00f;
    lab_course.lab_flag = 1;

    if (!add_parameter(&lecture_course.lecture, "Seatwork", 100.00f, 0.00f, lecture_only_activities, 1)
        || !add_parameter(&lab_course.lecture, "Exercises", 50.00f, 0.00f, mixed_lecture_activities, 2)
        || !add_parameter(&lab_course.lab, "Projects", 50.00f, 0.00f, mixed_lab_activities, 1)
        || !add_course(profile, &lecture_course)
        || !add_course(profile, &lab_course)) {
        course_reset(&lecture_course);
        course_reset(&lab_course);
        student_profile_reset(profile);
        return 0;
    }

    return 1;
}

static void expect_parameter_equal(TestContext* context, const Course_Parameter* expected, const Course_Parameter* actual, const char* message_prefix)
{
    size_t i;
    char message[128];

    expect_string_equal(context, expected->parameter_name, actual->parameter_name, message_prefix);
    expect_float_close(context, expected->weight, actual->weight, "parameter weight should round-trip");
    expect_float_close(context, expected->percentage, actual->percentage, "parameter percentage should round-trip");
    TEST_EXPECT_SIZE_EQ(context, vector_size(expected->activities), vector_size(actual->activities), "activity count should round-trip");

    for (i = 0; i < vector_size(expected->activities) && i < vector_size(actual->activities); i++) {
        Activities* expected_activity = (Activities*)vector_at(expected->activities, i);
        Activities* actual_activity = (Activities*)vector_at(actual->activities, i);

        snprintf(message, sizeof(message), "%s activity name should round-trip", message_prefix);
        expect_string_equal(context, expected_activity->activity_name, actual_activity->activity_name, message);
        expect_float_close(context, expected_activity->score, actual_activity->score, "activity score should round-trip");
        expect_float_close(context, expected_activity->total_score, actual_activity->total_score, "activity total score should round-trip");
    }
}

static void expect_profile_equal(TestContext* context, const Student_Profile* expected, const Student_Profile* actual)
{
    size_t i;

    expect_string_equal(context, expected->first_name, actual->first_name, "first name should round-trip");
    expect_string_equal(context, expected->middle_name, actual->middle_name, "middle name should round-trip");
    expect_string_equal(context, expected->last_name, actual->last_name, "last name should round-trip");
    expect_string_equal(context, expected->student_number, actual->student_number, "student number should round-trip");
    expect_string_equal(context, expected->degree_program, actual->degree_program, "degree program should round-trip");
    expect_float_close(context, expected->predicted_gwa, actual->predicted_gwa, "predicted GWA should round-trip");
    expect_float_close(context, expected->goal, actual->goal, "goal should round-trip");
    TEST_EXPECT_SIZE_EQ(context, vector_size(expected->courses), vector_size(actual->courses), "course count should round-trip");

    for (i = 0; i < vector_size(expected->courses) && i < vector_size(actual->courses); i++) {
        Course* expected_course = (Course*)vector_at(expected->courses, i);
        Course* actual_course = (Course*)vector_at(actual->courses, i);
        size_t parameter_index;

        expect_string_equal(context, expected_course->name, actual_course->name, "course name should round-trip");
        expect_float_close(context, expected_course->units, actual_course->units, "course units should round-trip");
        TEST_EXPECT(context, expected_course->lab_flag == actual_course->lab_flag, "lab flag should round-trip");
        TEST_EXPECT_SIZE_EQ(context, vector_size(expected_course->lecture.parameters), vector_size(actual_course->lecture.parameters), "lecture parameter count should round-trip");
        TEST_EXPECT_SIZE_EQ(context, vector_size(expected_course->lab.parameters), vector_size(actual_course->lab.parameters), "lab parameter count should round-trip");

        for (parameter_index = 0; parameter_index < vector_size(expected_course->lecture.parameters) && parameter_index < vector_size(actual_course->lecture.parameters); parameter_index++) {
            expect_parameter_equal(
                context,
                (Course_Parameter*)vector_at(expected_course->lecture.parameters, parameter_index),
                (Course_Parameter*)vector_at(actual_course->lecture.parameters, parameter_index),
                "lecture parameter"
            );
        }

        for (parameter_index = 0; parameter_index < vector_size(expected_course->lab.parameters) && parameter_index < vector_size(actual_course->lab.parameters); parameter_index++) {
            expect_parameter_equal(
                context,
                (Course_Parameter*)vector_at(expected_course->lab.parameters, parameter_index),
                (Course_Parameter*)vector_at(actual_course->lab.parameters, parameter_index),
                "lab parameter"
            );
        }
    }
}

static int file_contains_text(const char* path, const char* text)
{
    char line[256];
    FILE* file = fopen(path, "r");

    if (!file) {
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, text) != NULL) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

static void run_round_trip_case(TestContext* context, const char* path, int expect_lab_parameter, int (*builder)(Student_Profile*))
{
    Student_Profile expected;
    Student_Profile actual;
    PersistenceStatus status;

    TEST_EXPECT(context, builder(&expected), "profile builder should succeed");
    if (context->failures != 0) {
        return;
    }

    status = persistence_save_student_profile(&expected, path);
    TEST_EXPECT(context, status == PERSISTENCE_STATUS_OK, "persistence_save_student_profile should succeed");

    status = persistence_load_student_profile(path, &actual);
    TEST_EXPECT(context, status == PERSISTENCE_STATUS_OK, "persistence_load_student_profile should succeed");

    if (status == PERSISTENCE_STATUS_OK) {
        expect_profile_equal(context, &expected, &actual);
        TEST_EXPECT(context, file_contains_text(path, "GRADE_GUARD_CSV,1"), "saved file should include the versioned schema header");
        TEST_EXPECT(context, file_contains_text(path, "PROFILE,"), "saved file should include a PROFILE record");
        TEST_EXPECT(context, file_contains_text(path, "COURSE,"), "saved file should include a COURSE record");
        TEST_EXPECT(context, file_contains_text(path, "PARAMETER,lecture,"), "saved file should include lecture parameter records");
        TEST_EXPECT(context, file_contains_text(path, "ACTIVITY,"), "saved file should include activity records");
        TEST_EXPECT(context, file_contains_text(path, "PARAMETER,lab,") == expect_lab_parameter, "lab parameter records should match the profile shape");
        student_profile_reset(&actual);
    }

    student_profile_reset(&expected);
}

static void write_legacy_fixture(const char* path)
{
    FILE* file = fopen(path, "w");

    if (!file) {
        return;
    }

    fprintf(file, "First Name,Middle Name,Last Name,Student Number,Degree Program,Predicted GWA,Goal\n");
    fprintf(file, "Legacy,,Profile,2024-42069,BS Computer Science,3.08,1.20\n");
    fprintf(file, "Courses\n");
    fprintf(file, "CMSC 142,3.00,No\n");
    fprintf(file, "Lecture Parameters\n");
    fprintf(file, "Basic Quiz,50.00,0.00\n");
    fprintf(file, "Quiz 1,5.00,10.00\n");
    fprintf(file, "Quiz 2,10.00,10.00\n");
    fprintf(file, "Exam,50.00,0.00\n");
    fprintf(file, "Final Exam,100.00,100.00\n");
    fclose(file);
}

static void test_legacy_load(TestContext* context)
{
    Student_Profile profile;
    PersistenceStatus status;
    Course* course;

    write_legacy_fixture("grade-guard/unit-tests/persistence-legacy-fixture.csv");
    status = persistence_load_student_profile("grade-guard/unit-tests/persistence-legacy-fixture.csv", &profile);
    TEST_EXPECT(context, status == PERSISTENCE_STATUS_OK, "legacy fixture should still load");
    if (status != PERSISTENCE_STATUS_OK) {
        return;
    }

    expect_string_equal(context, "Legacy", profile.first_name, "legacy first name should load");
    TEST_EXPECT_SIZE_EQ(context, 1, vector_size(profile.courses), "legacy fixture should contain one course");

    course = (Course*)vector_at(profile.courses, 0);
    TEST_EXPECT(context, course != NULL, "legacy course should be available");
    TEST_EXPECT(context, course->lab_flag == 0, "legacy course lab flag should load");
    TEST_EXPECT_SIZE_EQ(context, 2, vector_size(course->lecture.parameters), "legacy fixture should recover two lecture parameters");

    student_profile_reset(&profile);
}

static void test_profile_listing(TestContext* context)
{
    Student_Profile first_profile;
    Student_Profile second_profile;
    Vector* paths;
    char* expected_first_path;
    char* expected_second_path;

    MAKE_DIRECTORY("grade-guard/unit-tests/persistence-runtime");

    TEST_EXPECT(context, build_shape_lecture_only(&first_profile), "first listing profile should build");
    TEST_EXPECT(context, build_shape_lab(&second_profile), "second listing profile should build");
    if (context->failures != 0) {
        return;
    }

    TEST_EXPECT(
        context,
        persistence_save_student_profile(&first_profile, "grade-guard/unit-tests/persistence-runtime/0.csv") == PERSISTENCE_STATUS_OK,
        "first runtime profile should save"
    );
    TEST_EXPECT(
        context,
        persistence_save_student_profile(&second_profile, "grade-guard/unit-tests/persistence-runtime/1.csv") == PERSISTENCE_STATUS_OK,
        "second runtime profile should save"
    );

    paths = persistence_list_profile_file_paths("grade-guard/unit-tests/persistence-runtime");
    TEST_EXPECT(context, paths != NULL, "persistence_list_profile_file_paths should return a vector");
    if (!paths) {
        student_profile_reset(&first_profile);
        student_profile_reset(&second_profile);
        return;
    }

    TEST_EXPECT_SIZE_EQ(context, 2, vector_size(paths), "profile listing should discover contiguous numbered files");
    expected_first_path = *(char**)vector_at(paths, 0);
    expected_second_path = *(char**)vector_at(paths, 1);
    expect_string_equal(context, "grade-guard/unit-tests/persistence-runtime/0.csv", expected_first_path, "first listed path should match the numbering contract");
    expect_string_equal(context, "grade-guard/unit-tests/persistence-runtime/1.csv", expected_second_path, "second listed path should match the numbering contract");

    persistence_destroy_profile_file_paths(paths);
    student_profile_reset(&first_profile);
    student_profile_reset(&second_profile);
}

int main(void)
{
    TestContext context = {0};

    run_round_trip_case(&context, "grade-guard/unit-tests/persistence-shape-lecture.csv", 0, build_shape_lecture_only);
    run_round_trip_case(&context, "grade-guard/unit-tests/persistence-shape-lab.csv", 1, build_shape_lab);
    run_round_trip_case(&context, "grade-guard/unit-tests/persistence-shape-mixed.csv", 1, build_shape_mixed);
    test_legacy_load(&context);
    test_profile_listing(&context);

    if (context.failures != 0) {
        fprintf(stderr, "persistence contract tests failed: %d\n", context.failures);
        return 1;
    }

    printf("persistence contract tests passed\n");
    return 0;
}
