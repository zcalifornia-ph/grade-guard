#include "persistence.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static char* persistence_copy_token(char* token)
{
    persistence_trim_newline(token);
    return persistence_strdup(token);
}

void save_student_profile_to_csv(const Student_Profile* profile, const char* path)
{
    size_t i;
    size_t j;
    size_t k;
    FILE* file;

    if (!profile || !path) {
        return;
    }

    file = fopen(path, "w");
    if (!file) {
        return;
    }

    fprintf(file, "First Name,Middle Name,Last Name,Student Number,Degree Program,Predicted GWA,Goal\n");
    fprintf(
        file,
        "%s,%s,%s,%s,%s,%.2f,%.2f\n",
        profile->first_name ? profile->first_name : "N/A",
        profile->middle_name ? profile->middle_name : "N/A",
        profile->last_name ? profile->last_name : "N/A",
        profile->student_number ? profile->student_number : "N/A",
        profile->degree_program ? profile->degree_program : "N/A",
        profile->predicted_gwa,
        profile->goal
    );

    fprintf(file, "Courses\n");
    for (i = 0; i < vector_size(profile->courses); i++) {
        Course* course = (Course*)vector_at(profile->courses, i);

        if (!course) {
            continue;
        }

        fprintf(
            file,
            "%s,%.2f,%s\n",
            course->name ? course->name : "N/A",
            course->units,
            course->lab_flag ? "Yes" : "No"
        );

        fprintf(file, "Lecture Parameters\n");
        for (j = 0; j < vector_size(course->lecture.parameters); j++) {
            Course_Parameter* parameter = (Course_Parameter*)vector_at(course->lecture.parameters, j);

            if (!parameter) {
                continue;
            }

            fprintf(
                file,
                "%s,%.2f,%.2f\n",
                parameter->parameter_name ? parameter->parameter_name : "N/A",
                parameter->weight,
                parameter->percentage
            );

            for (k = 0; k < vector_size(parameter->activities); k++) {
                Activities* activity = (Activities*)vector_at(parameter->activities, k);

                if (!activity) {
                    continue;
                }

                fprintf(
                    file,
                    "%s,%.2f,%.2f\n",
                    activity->activity_name ? activity->activity_name : "N/A",
                    activity->score,
                    activity->total_score
                );
            }
        }

        if (course->lab_flag) {
            fprintf(file, "Lab Parameters\n");
            for (j = 0; j < vector_size(course->lab.parameters); j++) {
                Course_Parameter* parameter = (Course_Parameter*)vector_at(course->lab.parameters, j);

                if (!parameter) {
                    continue;
                }

                fprintf(
                    file,
                    "%s,%.2f,%.2f\n",
                    parameter->parameter_name ? parameter->parameter_name : "N/A",
                    parameter->weight,
                    parameter->percentage
                );

                for (k = 0; k < vector_size(parameter->activities); k++) {
                    Activities* activity = (Activities*)vector_at(parameter->activities, k);

                    if (!activity) {
                        continue;
                    }

                    fprintf(
                        file,
                        "%s,%.2f,%.2f\n",
                        activity->activity_name ? activity->activity_name : "N/A",
                        activity->score,
                        activity->total_score
                    );
                }
            }
        }
    }

    fclose(file);
}

Student_Profile load_student_profile_from_csv(const char* path)
{
    Student_Profile profile = {0};
    FILE* file;
    char line[256];
    char* token;

    if (!student_profile_init(&profile)) {
        return profile;
    }

    if (!path) {
        return profile;
    }

    file = fopen(path, "r");
    if (!file) {
        return profile;
    }

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return profile;
    }

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return profile;
    }

    token = strtok(line, ",");
    if (token) {
        profile.first_name = persistence_copy_token(token);
    }

    token = strtok(NULL, ",");
    if (token) {
        profile.middle_name = persistence_copy_token(token);
    }

    token = strtok(NULL, ",");
    if (token) {
        profile.last_name = persistence_copy_token(token);
    }

    token = strtok(NULL, ",");
    if (token) {
        profile.student_number = persistence_copy_token(token);
    }

    token = strtok(NULL, ",");
    if (token) {
        profile.degree_program = persistence_copy_token(token);
    }

    token = strtok(NULL, ",");
    if (token) {
        profile.predicted_gwa = (float)atof(token);
    }

    token = strtok(NULL, ",");
    if (token) {
        profile.goal = (float)atof(token);
    }

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return profile;
    }

    while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
        Course course;

        if (!course_init(&course)) {
            fclose(file);
            return profile;
        }

        token = strtok(line, ",");
        if (token) {
            course.name = persistence_copy_token(token);
        }

        token = strtok(NULL, ",");
        if (token) {
            course.units = (float)atof(token);
        }

        token = strtok(NULL, ",");
        if (token) {
            persistence_trim_newline(token);
            course.lab_flag = strcmp(token, "Yes") == 0;
        }

        if (!fgets(line, sizeof(line), file)) {
            vector_push_back(profile.courses, &course);
            break;
        }

        while (fgets(line, sizeof(line), file) && strcmp(line, "Lab Parameters\n") != 0 && strcmp(line, "\n") != 0) {
            Course_Parameter parameter;

            if (!course_parameter_init(&parameter)) {
                course_reset(&course);
                fclose(file);
                return profile;
            }

            token = strtok(line, ",");
            if (token) {
                parameter.parameter_name = persistence_copy_token(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                parameter.weight = (float)atof(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                parameter.percentage = (float)atof(token);
            }

            while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
                Activities activity;

                if (!activities_init(&activity)) {
                    continue;
                }

                token = strtok(line, ",");
                if (token) {
                    activity.activity_name = persistence_copy_token(token);
                }

                token = strtok(NULL, ",");
                if (token) {
                    activity.score = (float)atof(token);
                }

                token = strtok(NULL, ",");
                if (token) {
                    activity.total_score = (float)atof(token);
                }

                vector_push_back(parameter.activities, &activity);
            }

            vector_push_back(course.lecture.parameters, &parameter);
        }

        if (course.lab_flag) {
            while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
                Course_Parameter parameter;

                if (!course_parameter_init(&parameter)) {
                    course_reset(&course);
                    fclose(file);
                    return profile;
                }

                token = strtok(line, ",");
                if (token) {
                    parameter.parameter_name = persistence_copy_token(token);
                }

                token = strtok(NULL, ",");
                if (token) {
                    parameter.weight = (float)atof(token);
                }

                token = strtok(NULL, ",");
                if (token) {
                    parameter.percentage = (float)atof(token);
                }

                while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
                    Activities activity;

                    if (!activities_init(&activity)) {
                        continue;
                    }

                    token = strtok(line, ",");
                    if (token) {
                        activity.activity_name = persistence_copy_token(token);
                    }

                    token = strtok(NULL, ",");
                    if (token) {
                        activity.score = (float)atof(token);
                    }

                    token = strtok(NULL, ",");
                    if (token) {
                        activity.total_score = (float)atof(token);
                    }

                    vector_push_back(parameter.activities, &activity);
                }

                vector_push_back(course.lab.parameters, &parameter);
            }
        }

        vector_push_back(profile.courses, &course);
    }

    fclose(file);
    return profile;
}

int file_exists(const char* filename)
{
    FILE* file;

    if (!filename) {
        return 0;
    }

    file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }

    return 0;
}

char* append_size_to_csv(size_t size)
{
    char* result;
    size_t size_length;

    size_length = (size_t)snprintf(NULL, 0, "%zu", size);
    result = (char*)malloc(size_length + 5);
    if (!result) {
        return NULL;
    }

    snprintf(result, size_length + 1, "%zu", size);
    snprintf(result + size_length, 5, ".csv");
    return result;
}
