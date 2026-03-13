#include "persistence.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERSISTENCE_LINE_BUFFER_SIZE ((size_t)1024)
#define PERSISTENCE_MAX_FIELDS ((size_t)8)

static const char kPersistenceHeader[] = "GRADE_GUARD_CSV,1";
static const char kProfileRecord[] = "PROFILE";
static const char kCourseRecord[] = "COURSE";
static const char kParameterRecord[] = "PARAMETER";
static const char kActivityRecord[] = "ACTIVITY";
static const char kLectureComponent[] = "lecture";
static const char kLabComponent[] = "lab";
static const char kLegacyCoursesHeader[] = "Courses";
static const char kLegacyLectureHeader[] = "Lecture Parameters";
static const char kLegacyLabHeader[] = "Lab Parameters";
static const char kLegacyMissingText[] = "N/A";

typedef enum PersistenceComponentKind {
    PERSISTENCE_COMPONENT_NONE = 0,
    PERSISTENCE_COMPONENT_LECTURE,
    PERSISTENCE_COMPONENT_LAB
} PersistenceComponentKind;

static char* persistence_strdup(const char* value)
{
    char* copy;
    size_t length;

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

static void persistence_trim_newline(char* value)
{
    size_t length;

    if (!value) {
        return;
    }

    length = strlen(value);
    while (length > 0 && (value[length - 1] == '\n' || value[length - 1] == '\r')) {
        value[length - 1] = '\0';
        length--;
    }
}

static size_t persistence_split_line(char* line, char* fields[], size_t max_fields)
{
    char* cursor;
    size_t count;

    if (!line || !fields || max_fields == 0) {
        return 0;
    }

    persistence_trim_newline(line);

    fields[0] = line;
    count = 1;

    for (cursor = line; *cursor != '\0'; cursor++) {
        if (*cursor == ',') {
            *cursor = '\0';
            if (count < max_fields) {
                fields[count] = cursor + 1;
            }
            count++;
        }
    }

    return count;
}

static int persistence_push_element(Vector* vector, const void* element)
{
    size_t previous_size;

    if (!vector || !element) {
        return 0;
    }

    previous_size = vector_size(vector);
    vector_push_back(vector, element);
    return vector_size(vector) == previous_size + 1;
}

static int persistence_parse_float_token(const char* token, float* value)
{
    char* end;

    if (!token || !value || token[0] == '\0') {
        return 0;
    }

    *value = strtof(token, &end);
    return end != token && end != NULL && *end == '\0';
}

static int persistence_parse_v1_lab_flag(const char* token, bool* lab_flag)
{
    if (!token || !lab_flag) {
        return 0;
    }

    if (strcmp(token, "0") == 0) {
        *lab_flag = false;
        return 1;
    }

    if (strcmp(token, "1") == 0) {
        *lab_flag = true;
        return 1;
    }

    return 0;
}

static int persistence_parse_legacy_lab_flag(const char* token, bool* lab_flag)
{
    if (!token || !lab_flag) {
        return 0;
    }

    if (strcmp(token, "No") == 0) {
        *lab_flag = false;
        return 1;
    }

    if (strcmp(token, "Yes") == 0) {
        *lab_flag = true;
        return 1;
    }

    return 0;
}

static int persistence_text_supported(const char* value)
{
    size_t i;

    if (!value) {
        return 1;
    }

    for (i = 0; value[i] != '\0'; i++) {
        if (value[i] == ',' || value[i] == '\n' || value[i] == '\r') {
            return 0;
        }
    }

    return 1;
}

static char* persistence_copy_text(const char* token)
{
    if (!token) {
        return NULL;
    }

    return persistence_strdup(token);
}

static char* persistence_copy_legacy_text(const char* token)
{
    if (!token) {
        return NULL;
    }

    if (strcmp(token, kLegacyMissingText) == 0) {
        return persistence_strdup("");
    }

    return persistence_strdup(token);
}

static PersistenceStatus persistence_assign_text(char** target, const char* token)
{
    if (!target || !token) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    *target = persistence_copy_text(token);
    return *target ? PERSISTENCE_STATUS_OK : PERSISTENCE_STATUS_MEMORY_ERROR;
}

static PersistenceStatus persistence_assign_legacy_text(char** target, const char* token)
{
    if (!target || !token) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    *target = persistence_copy_legacy_text(token);
    return *target ? PERSISTENCE_STATUS_OK : PERSISTENCE_STATUS_MEMORY_ERROR;
}

static int persistence_file_exists(const char* path)
{
    FILE* file;

    if (!path) {
        return 0;
    }

    file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    fclose(file);
    return 1;
}

static void persistence_zero_course(Course* course)
{
    if (!course) {
        return;
    }

    memset(course, 0, sizeof(*course));
}

static void persistence_zero_parameter(Course_Parameter* parameter)
{
    if (!parameter) {
        return;
    }

    memset(parameter, 0, sizeof(*parameter));
}

static PersistenceStatus persistence_commit_parameter(Course_Component* component, Course_Parameter* parameter, int* has_parameter)
{
    if (!parameter || !has_parameter) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    if (!*has_parameter) {
        return PERSISTENCE_STATUS_OK;
    }

    if (!component) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    if (!component->parameters || !persistence_push_element(component->parameters, parameter)) {
        course_parameter_reset(parameter);
        persistence_zero_parameter(parameter);
        *has_parameter = 0;
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    persistence_zero_parameter(parameter);
    *has_parameter = 0;
    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_commit_course(Student_Profile* profile, Course* course, int* has_course)
{
    if (!profile || !course || !has_course) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    if (!*has_course) {
        return PERSISTENCE_STATUS_OK;
    }

    if (!profile->courses || !persistence_push_element(profile->courses, course)) {
        course_reset(course);
        persistence_zero_course(course);
        *has_course = 0;
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    persistence_zero_course(course);
    *has_course = 0;
    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_add_activity(Course_Parameter* parameter, const char* name, float score, float total_score, int legacy_mode)
{
    Activities activity;

    if (!parameter || !parameter->activities || !name) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    if (!activities_init(&activity)) {
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    activity.activity_name = legacy_mode ? persistence_copy_legacy_text(name) : persistence_copy_text(name);
    if (!activity.activity_name) {
        activities_reset(&activity);
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    activity.score = score;
    activity.total_score = total_score;

    if (!persistence_push_element(parameter->activities, &activity)) {
        activities_reset(&activity);
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_parse_profile_record(Student_Profile* profile, char* fields[], size_t field_count)
{
    PersistenceStatus status;

    if (!profile || !fields || field_count != 8 || strcmp(fields[0], kProfileRecord) != 0) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    status = persistence_assign_text(&profile->first_name, fields[1]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_text(&profile->middle_name, fields[2]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_text(&profile->last_name, fields[3]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_text(&profile->student_number, fields[4]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_text(&profile->degree_program, fields[5]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    if (!persistence_parse_float_token(fields[6], &profile->predicted_gwa)) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!persistence_parse_float_token(fields[7], &profile->goal)) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_parse_legacy_profile_row(Student_Profile* profile, char* fields[], size_t field_count)
{
    PersistenceStatus status;

    if (!profile || !fields || field_count != 7) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    status = persistence_assign_legacy_text(&profile->first_name, fields[0]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_legacy_text(&profile->middle_name, fields[1]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_legacy_text(&profile->last_name, fields[2]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_legacy_text(&profile->student_number, fields[3]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    status = persistence_assign_legacy_text(&profile->degree_program, fields[4]);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    if (!persistence_parse_float_token(fields[5], &profile->predicted_gwa)) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!persistence_parse_float_token(fields[6], &profile->goal)) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_init_course_from_v1(Course* course, char* fields[], size_t field_count)
{
    PersistenceStatus status;

    if (!course || !fields || field_count != 4 || strcmp(fields[0], kCourseRecord) != 0) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!course_init(course)) {
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    status = persistence_assign_text(&course->name, fields[1]);
    if (status != PERSISTENCE_STATUS_OK) {
        course_reset(course);
        persistence_zero_course(course);
        return status;
    }

    if (!persistence_parse_float_token(fields[2], &course->units) || !persistence_parse_v1_lab_flag(fields[3], &course->lab_flag)) {
        course_reset(course);
        persistence_zero_course(course);
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_init_course_from_legacy(Course* course, char* fields[], size_t field_count)
{
    PersistenceStatus status;

    if (!course || !fields || field_count != 3) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!course_init(course)) {
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    status = persistence_assign_legacy_text(&course->name, fields[0]);
    if (status != PERSISTENCE_STATUS_OK) {
        course_reset(course);
        persistence_zero_course(course);
        return status;
    }

    if (!persistence_parse_float_token(fields[1], &course->units) || !persistence_parse_legacy_lab_flag(fields[2], &course->lab_flag)) {
        course_reset(course);
        persistence_zero_course(course);
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_init_parameter_from_v1(
    Course_Parameter* parameter,
    PersistenceComponentKind* component_kind,
    char* fields[],
    size_t field_count
)
{
    PersistenceStatus status;

    if (!parameter || !component_kind || !fields || field_count != 5 || strcmp(fields[0], kParameterRecord) != 0) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (strcmp(fields[1], kLectureComponent) == 0) {
        *component_kind = PERSISTENCE_COMPONENT_LECTURE;
    } else if (strcmp(fields[1], kLabComponent) == 0) {
        *component_kind = PERSISTENCE_COMPONENT_LAB;
    } else {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!course_parameter_init(parameter)) {
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    status = persistence_assign_text(&parameter->parameter_name, fields[2]);
    if (status != PERSISTENCE_STATUS_OK) {
        course_parameter_reset(parameter);
        persistence_zero_parameter(parameter);
        return status;
    }

    if (!persistence_parse_float_token(fields[3], &parameter->weight) || !persistence_parse_float_token(fields[4], &parameter->percentage)) {
        course_parameter_reset(parameter);
        persistence_zero_parameter(parameter);
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_init_parameter_from_legacy(Course_Parameter* parameter, char* fields[], size_t field_count)
{
    PersistenceStatus status;

    if (!parameter || !fields || field_count != 3) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!course_parameter_init(parameter)) {
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    status = persistence_assign_legacy_text(&parameter->parameter_name, fields[0]);
    if (status != PERSISTENCE_STATUS_OK) {
        course_parameter_reset(parameter);
        persistence_zero_parameter(parameter);
        return status;
    }

    if (!persistence_parse_float_token(fields[1], &parameter->weight) || !persistence_parse_float_token(fields[2], &parameter->percentage)) {
        course_parameter_reset(parameter);
        persistence_zero_parameter(parameter);
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    return PERSISTENCE_STATUS_OK;
}

static PersistenceStatus persistence_parse_v1(FILE* file, Student_Profile* profile)
{
    char line[PERSISTENCE_LINE_BUFFER_SIZE];
    char* fields[PERSISTENCE_MAX_FIELDS];
    Course current_course;
    Course_Parameter current_parameter;
    Course_Component* current_component;
    int have_profile;
    int have_course;
    int have_parameter;
    PersistenceStatus status;

    persistence_zero_course(&current_course);
    persistence_zero_parameter(&current_parameter);
    current_component = NULL;
    have_profile = 0;
    have_course = 0;
    have_parameter = 0;
    status = PERSISTENCE_STATUS_OK;

    while (fgets(line, sizeof(line), file)) {
        size_t field_count;
        PersistenceComponentKind component_kind;

        persistence_trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        field_count = persistence_split_line(line, fields, PERSISTENCE_MAX_FIELDS);
        if (field_count == 0) {
            continue;
        }

        if (strcmp(fields[0], kProfileRecord) == 0) {
            if (have_profile) {
                status = PERSISTENCE_STATUS_PARSE_ERROR;
                break;
            }

            status = persistence_parse_profile_record(profile, fields, field_count);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            have_profile = 1;
            continue;
        }

        if (!have_profile) {
            status = PERSISTENCE_STATUS_PARSE_ERROR;
            break;
        }

        if (strcmp(fields[0], kCourseRecord) == 0) {
            status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            current_component = NULL;

            status = persistence_commit_course(profile, &current_course, &have_course);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            status = persistence_init_course_from_v1(&current_course, fields, field_count);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            have_course = 1;
            continue;
        }

        if (strcmp(fields[0], kParameterRecord) == 0) {
            if (!have_course) {
                status = PERSISTENCE_STATUS_PARSE_ERROR;
                break;
            }

            status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            status = persistence_init_parameter_from_v1(&current_parameter, &component_kind, fields, field_count);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            if (component_kind == PERSISTENCE_COMPONENT_LAB) {
                if (!current_course.lab_flag) {
                    course_parameter_reset(&current_parameter);
                    persistence_zero_parameter(&current_parameter);
                    status = PERSISTENCE_STATUS_PARSE_ERROR;
                    break;
                }

                current_component = &current_course.lab;
            } else {
                current_component = &current_course.lecture;
            }

            have_parameter = 1;
            continue;
        }

        if (strcmp(fields[0], kActivityRecord) == 0) {
            float score;
            float total_score;

            if (!have_parameter || !current_component || field_count != 4) {
                status = PERSISTENCE_STATUS_PARSE_ERROR;
                break;
            }

            if (!persistence_parse_float_token(fields[2], &score) || !persistence_parse_float_token(fields[3], &total_score)) {
                status = PERSISTENCE_STATUS_PARSE_ERROR;
                break;
            }

            status = persistence_add_activity(&current_parameter, fields[1], score, total_score, 0);
            if (status != PERSISTENCE_STATUS_OK) {
                break;
            }

            continue;
        }

        status = PERSISTENCE_STATUS_PARSE_ERROR;
        break;
    }

    if (status == PERSISTENCE_STATUS_OK) {
        if (!have_profile) {
            status = PERSISTENCE_STATUS_PARSE_ERROR;
        } else {
            status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
        }
    }

    if (status == PERSISTENCE_STATUS_OK) {
        status = persistence_commit_course(profile, &current_course, &have_course);
    }

    if (status != PERSISTENCE_STATUS_OK) {
        if (have_parameter) {
            course_parameter_reset(&current_parameter);
        }
        if (have_course) {
            course_reset(&current_course);
        }
    }

    return status;
}

static PersistenceStatus persistence_parse_legacy(FILE* file, Student_Profile* profile)
{
    char line[PERSISTENCE_LINE_BUFFER_SIZE];
    char* fields[PERSISTENCE_MAX_FIELDS];
    size_t field_count;
    Course current_course;
    Course_Parameter current_parameter;
    Course_Component* current_component;
    int have_course;
    int have_parameter;
    int expecting_lecture_header;
    PersistenceStatus status;

    persistence_zero_course(&current_course);
    persistence_zero_parameter(&current_parameter);
    current_component = NULL;
    have_course = 0;
    have_parameter = 0;
    expecting_lecture_header = 0;
    status = PERSISTENCE_STATUS_OK;

    if (!fgets(line, sizeof(line), file)) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    if (!fgets(line, sizeof(line), file)) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    field_count = persistence_split_line(line, fields, PERSISTENCE_MAX_FIELDS);
    status = persistence_parse_legacy_profile_row(profile, fields, field_count);
    if (status != PERSISTENCE_STATUS_OK) {
        return status;
    }

    if (!fgets(line, sizeof(line), file)) {
        return PERSISTENCE_STATUS_OK;
    }

    persistence_trim_newline(line);
    if (strcmp(line, kLegacyCoursesHeader) != 0) {
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    while (fgets(line, sizeof(line), file)) {
        persistence_trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        if (expecting_lecture_header) {
            if (strcmp(line, kLegacyLectureHeader) != 0) {
                status = PERSISTENCE_STATUS_PARSE_ERROR;
                break;
            }

            current_component = &current_course.lecture;
            expecting_lecture_header = 0;
            continue;
        }

        if (strcmp(line, kLegacyLabHeader) == 0) {
            status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
            if (status != PERSISTENCE_STATUS_OK || !have_course || !current_course.lab_flag) {
                if (status == PERSISTENCE_STATUS_OK) {
                    status = PERSISTENCE_STATUS_PARSE_ERROR;
                }
                break;
            }

            current_component = &current_course.lab;
            continue;
        }

        field_count = persistence_split_line(line, fields, PERSISTENCE_MAX_FIELDS);
        if (field_count == 3) {
            bool lab_flag;

            if (persistence_parse_legacy_lab_flag(fields[2], &lab_flag)) {
                status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
                if (status != PERSISTENCE_STATUS_OK) {
                    break;
                }

                current_component = NULL;

                status = persistence_commit_course(profile, &current_course, &have_course);
                if (status != PERSISTENCE_STATUS_OK) {
                    break;
                }

                status = persistence_init_course_from_legacy(&current_course, fields, field_count);
                if (status != PERSISTENCE_STATUS_OK) {
                    break;
                }

                have_course = 1;
                expecting_lecture_header = 1;
                continue;
            }
        }

        if (!have_course || !current_component || field_count != 3) {
            status = PERSISTENCE_STATUS_PARSE_ERROR;
            break;
        }

        {
            float second_value;
            float third_value;

            if (!persistence_parse_float_token(fields[1], &second_value) || !persistence_parse_float_token(fields[2], &third_value)) {
                status = PERSISTENCE_STATUS_PARSE_ERROR;
                break;
            }

            /*
             * Pre-Bolt-4.1 files used unlabeled 3-field rows for both parameters
             * and activities. Historical writers stored parameter percentage as
             * 0.00, so that value is the compatibility boundary for old files.
             */
            if (!have_parameter || third_value <= 0.0f) {
                status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
                if (status != PERSISTENCE_STATUS_OK) {
                    break;
                }

                status = persistence_init_parameter_from_legacy(&current_parameter, fields, field_count);
                if (status != PERSISTENCE_STATUS_OK) {
                    break;
                }

                have_parameter = 1;
            } else {
                status = persistence_add_activity(&current_parameter, fields[0], second_value, third_value, 1);
                if (status != PERSISTENCE_STATUS_OK) {
                    break;
                }
            }
        }
    }

    if (status == PERSISTENCE_STATUS_OK) {
        status = persistence_commit_parameter(current_component, &current_parameter, &have_parameter);
    }

    if (status == PERSISTENCE_STATUS_OK) {
        status = persistence_commit_course(profile, &current_course, &have_course);
    }

    if (status != PERSISTENCE_STATUS_OK) {
        if (have_parameter) {
            course_parameter_reset(&current_parameter);
        }
        if (have_course) {
            course_reset(&current_course);
        }
    }

    return status;
}

static int persistence_profile_structure_valid(const Student_Profile* profile)
{
    size_t course_index;

    if (!profile || !profile->courses) {
        return 0;
    }

    for (course_index = 0; course_index < vector_size(profile->courses); course_index++) {
        Course* course = (Course*)vector_at(profile->courses, course_index);
        size_t parameter_index;

        if (!course || !course->lecture.parameters || !course->lab.parameters) {
            return 0;
        }

        if (!course->lab_flag && vector_size(course->lab.parameters) > 0) {
            return 0;
        }

        for (parameter_index = 0; parameter_index < vector_size(course->lecture.parameters); parameter_index++) {
            Course_Parameter* parameter = (Course_Parameter*)vector_at(course->lecture.parameters, parameter_index);

            if (!parameter || !parameter->activities) {
                return 0;
            }
        }

        for (parameter_index = 0; parameter_index < vector_size(course->lab.parameters); parameter_index++) {
            Course_Parameter* parameter = (Course_Parameter*)vector_at(course->lab.parameters, parameter_index);

            if (!parameter || !parameter->activities) {
                return 0;
            }
        }
    }

    return 1;
}

static int persistence_profile_text_supported(const Student_Profile* profile)
{
    size_t course_index;

    if (!profile) {
        return 0;
    }

    if (!persistence_text_supported(profile->first_name)
        || !persistence_text_supported(profile->middle_name)
        || !persistence_text_supported(profile->last_name)
        || !persistence_text_supported(profile->student_number)
        || !persistence_text_supported(profile->degree_program)) {
        return 0;
    }

    for (course_index = 0; course_index < vector_size(profile->courses); course_index++) {
        Course* course = (Course*)vector_at(profile->courses, course_index);
        size_t parameter_index;

        if (!course || !persistence_text_supported(course->name)) {
            return 0;
        }

        for (parameter_index = 0; parameter_index < vector_size(course->lecture.parameters); parameter_index++) {
            Course_Parameter* parameter = (Course_Parameter*)vector_at(course->lecture.parameters, parameter_index);
            size_t activity_index;

            if (!parameter || !persistence_text_supported(parameter->parameter_name)) {
                return 0;
            }

            for (activity_index = 0; activity_index < vector_size(parameter->activities); activity_index++) {
                Activities* activity = (Activities*)vector_at(parameter->activities, activity_index);

                if (!activity || !persistence_text_supported(activity->activity_name)) {
                    return 0;
                }
            }
        }

        for (parameter_index = 0; parameter_index < vector_size(course->lab.parameters); parameter_index++) {
            Course_Parameter* parameter = (Course_Parameter*)vector_at(course->lab.parameters, parameter_index);
            size_t activity_index;

            if (!parameter || !persistence_text_supported(parameter->parameter_name)) {
                return 0;
            }

            for (activity_index = 0; activity_index < vector_size(parameter->activities); activity_index++) {
                Activities* activity = (Activities*)vector_at(parameter->activities, activity_index);

                if (!activity || !persistence_text_supported(activity->activity_name)) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

static PersistenceStatus persistence_write_component(FILE* file, const char* component_name, Vector* parameters)
{
    size_t parameter_index;

    if (!file || !component_name || !parameters) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    for (parameter_index = 0; parameter_index < vector_size(parameters); parameter_index++) {
        Course_Parameter* parameter = (Course_Parameter*)vector_at(parameters, parameter_index);
        size_t activity_index;

        if (!parameter || !parameter->activities) {
            return PERSISTENCE_STATUS_INVALID_ARGUMENT;
        }

        if (fprintf(
            file,
            "%s,%s,%s,%.2f,%.2f\n",
            kParameterRecord,
            component_name,
            parameter->parameter_name ? parameter->parameter_name : "",
            parameter->weight,
            parameter->percentage
        ) < 0) {
            return PERSISTENCE_STATUS_IO_ERROR;
        }

        for (activity_index = 0; activity_index < vector_size(parameter->activities); activity_index++) {
            Activities* activity = (Activities*)vector_at(parameter->activities, activity_index);

            if (!activity) {
                return PERSISTENCE_STATUS_INVALID_ARGUMENT;
            }

            if (fprintf(
                file,
                "%s,%s,%.2f,%.2f\n",
                kActivityRecord,
                activity->activity_name ? activity->activity_name : "",
                activity->score,
                activity->total_score
            ) < 0) {
                return PERSISTENCE_STATUS_IO_ERROR;
            }
        }
    }

    return PERSISTENCE_STATUS_OK;
}

PersistenceStatus persistence_save_student_profile(const Student_Profile* profile, const char* path)
{
    size_t course_index;
    FILE* file;
    PersistenceStatus status;

    if (!profile || !path) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    if (!persistence_profile_structure_valid(profile)) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    if (!persistence_profile_text_supported(profile)) {
        return PERSISTENCE_STATUS_UNSUPPORTED_TEXT;
    }

    file = fopen(path, "w");
    if (!file) {
        return PERSISTENCE_STATUS_IO_ERROR;
    }

    status = PERSISTENCE_STATUS_OK;

    if (fprintf(file, "%s\n", kPersistenceHeader) < 0
        || fprintf(
            file,
            "%s,%s,%s,%s,%s,%s,%.2f,%.2f\n",
            kProfileRecord,
            profile->first_name ? profile->first_name : "",
            profile->middle_name ? profile->middle_name : "",
            profile->last_name ? profile->last_name : "",
            profile->student_number ? profile->student_number : "",
            profile->degree_program ? profile->degree_program : "",
            profile->predicted_gwa,
            profile->goal
        ) < 0) {
        status = PERSISTENCE_STATUS_IO_ERROR;
    }

    for (course_index = 0; status == PERSISTENCE_STATUS_OK && course_index < vector_size(profile->courses); course_index++) {
        Course* course = (Course*)vector_at(profile->courses, course_index);

        if (!course) {
            status = PERSISTENCE_STATUS_INVALID_ARGUMENT;
            break;
        }

        if (fprintf(
            file,
            "%s,%s,%.2f,%d\n",
            kCourseRecord,
            course->name ? course->name : "",
            course->units,
            course->lab_flag ? 1 : 0
        ) < 0) {
            status = PERSISTENCE_STATUS_IO_ERROR;
            break;
        }

        status = persistence_write_component(file, kLectureComponent, course->lecture.parameters);
        if (status != PERSISTENCE_STATUS_OK) {
            break;
        }

        if (course->lab_flag) {
            status = persistence_write_component(file, kLabComponent, course->lab.parameters);
        }
    }

    if (fclose(file) != 0 && status == PERSISTENCE_STATUS_OK) {
        status = PERSISTENCE_STATUS_IO_ERROR;
    }

    return status;
}

PersistenceStatus persistence_load_student_profile(const char* path, Student_Profile* profile)
{
    FILE* file;
    char first_line[PERSISTENCE_LINE_BUFFER_SIZE];
    PersistenceStatus status;

    if (!path || !profile) {
        return PERSISTENCE_STATUS_INVALID_ARGUMENT;
    }

    memset(profile, 0, sizeof(*profile));
    if (!student_profile_init(profile)) {
        return PERSISTENCE_STATUS_MEMORY_ERROR;
    }

    file = fopen(path, "r");
    if (!file) {
        student_profile_reset(profile);
        memset(profile, 0, sizeof(*profile));
        return PERSISTENCE_STATUS_IO_ERROR;
    }

    if (!fgets(first_line, sizeof(first_line), file)) {
        fclose(file);
        student_profile_reset(profile);
        memset(profile, 0, sizeof(*profile));
        return PERSISTENCE_STATUS_PARSE_ERROR;
    }

    persistence_trim_newline(first_line);
    if (strcmp(first_line, kPersistenceHeader) == 0) {
        status = persistence_parse_v1(file, profile);
    } else {
        rewind(file);
        status = persistence_parse_legacy(file, profile);
    }

    fclose(file);

    if (status != PERSISTENCE_STATUS_OK) {
        student_profile_reset(profile);
        memset(profile, 0, sizeof(*profile));
    }

    return status;
}

char* persistence_build_profile_path(const char* directory, size_t index)
{
    char* path;
    size_t directory_length;
    size_t index_length;
    int needs_separator;

    if (!directory || directory[0] == '\0') {
        return NULL;
    }

    directory_length = strlen(directory);
    needs_separator = directory[directory_length - 1] != '/' && directory[directory_length - 1] != '\\';
    index_length = (size_t)snprintf(NULL, 0, "%zu", index);
    path = (char*)malloc(directory_length + (size_t)needs_separator + index_length + 5);
    if (!path) {
        return NULL;
    }

    snprintf(
        path,
        directory_length + (size_t)needs_separator + index_length + 5,
        "%s%s%zu.csv",
        directory,
        needs_separator ? "/" : "",
        index
    );

    return path;
}

Vector* persistence_list_profile_file_paths(const char* directory)
{
    Vector* paths;
    size_t index;

    if (!directory) {
        return NULL;
    }

    paths = vector_create(sizeof(char*));
    if (!paths) {
        return NULL;
    }

    for (index = 0; ; index++) {
        char* path;

        path = persistence_build_profile_path(directory, index);
        if (!path) {
            persistence_destroy_profile_file_paths(paths);
            return NULL;
        }

        if (!persistence_file_exists(path)) {
            free(path);
            break;
        }

        if (!persistence_push_element(paths, &path)) {
            free(path);
            persistence_destroy_profile_file_paths(paths);
            return NULL;
        }
    }

    return paths;
}

void persistence_destroy_profile_file_paths(Vector* paths)
{
    size_t i;

    if (!paths) {
        return;
    }

    for (i = 0; i < vector_size(paths); i++) {
        char** path = (char**)vector_at(paths, i);

        if (path) {
            free(*path);
        }
    }

    vector_destroy(paths);
}
