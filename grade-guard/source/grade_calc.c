#include "grade_calc.h"

const float GRADE_GOALS[GRADE_GUARD_GRADE_GOAL_COUNT] = {
    3.00f,
    2.375f,
    1.75f,
    1.45f,
    1.20f
};

float percentage_to_gwa(float percentage)
{
    if (percentage < 0.0f) {
        percentage = 0.0f;
    } else if (percentage > 1.0f) {
        percentage = 1.0f;
    }

    return 1.0f + 4.0f * (1.0f - percentage);
}

float calculate_goal_percentage(float predicted_gwa, float goal_gwa)
{
    float normalized_predicted;
    float normalized_goal;

    normalized_predicted = 6.0f - predicted_gwa;
    normalized_goal = 6.0f - goal_gwa;

    if (normalized_goal > 0.0f) {
        float percentage = (normalized_predicted / normalized_goal) * 100.0f;

        if (percentage > 100.0f) {
            return 100.0f;
        }

        if (percentage < 0.0f) {
            return 0.0f;
        }

        return percentage;
    }

    return 0.0f;
}

float calculate_weighted_average(Student_Profile* profile)
{
    float gwa;
    float total_units;
    size_t i;
    size_t j;
    size_t k;

    if (!profile || !profile->courses || vector_size(profile->courses) == 0) {
        return 0.0f;
    }

    gwa = 0.0f;
    total_units = 0.0f;

    for (i = 0; i < vector_size(profile->courses); i++) {
        Course* course = (Course*)vector_at(profile->courses, i);

        if (course) {
            total_units += course->units;
        }
    }

    if (total_units <= 0.0f) {
        return 0.0f;
    }

    for (i = 0; i < vector_size(profile->courses); i++) {
        Course* course = (Course*)vector_at(profile->courses, i);
        float course_percentage;
        float lecture_percentage;
        float lab_percentage;
        Course_Component component;

        if (!course) {
            continue;
        }

        course_percentage = 0.0f;
        lecture_percentage = 0.0f;
        lab_percentage = 0.0f;
        component = course->lecture;

        for (j = 0; j < vector_size(component.parameters); j++) {
            Course_Parameter* parameter = (Course_Parameter*)vector_at(component.parameters, j);
            float parameter_total_score;
            float parameter_actual_score;
            size_t parameter_activity_count;

            if (!parameter || !parameter->activities) {
                continue;
            }

            parameter_total_score = 0.0f;
            parameter_actual_score = 0.0f;
            parameter_activity_count = vector_size(parameter->activities);

            for (k = 0; k < parameter_activity_count; k++) {
                Activities* activity = (Activities*)vector_at(parameter->activities, k);

                if (activity && activity->total_score > 0.0f && activity->score > 0.0f) {
                    parameter_actual_score += activity->score;
                    parameter_total_score += activity->total_score;
                }
            }

            if (parameter_activity_count > 0 && parameter_total_score > 0.0f) {
                float parameter_percentage = parameter_actual_score / parameter_total_score;

                lecture_percentage += parameter_percentage * (parameter->weight / 100.0f);
            }
        }

        if (course->lab_flag) {
            component = course->lab;

            for (j = 0; j < vector_size(component.parameters); j++) {
                Course_Parameter* parameter = (Course_Parameter*)vector_at(component.parameters, j);
                float parameter_total_score;
                float parameter_actual_score;
                size_t parameter_activity_count;

                if (!parameter || !parameter->activities) {
                    continue;
                }

                parameter_total_score = 0.0f;
                parameter_actual_score = 0.0f;
                parameter_activity_count = vector_size(parameter->activities);

                for (k = 0; k < parameter_activity_count; k++) {
                    Activities* activity = (Activities*)vector_at(parameter->activities, k);

                    if (activity && activity->total_score > 0.0f && activity->score > 0.0f) {
                        parameter_actual_score += activity->score;
                        parameter_total_score += activity->total_score;
                    }
                }

                if (parameter_activity_count > 0 && parameter_total_score > 0.0f) {
                    float parameter_percentage = parameter_actual_score / parameter_total_score;

                    lab_percentage += parameter_percentage * (parameter->weight / 100.0f);
                }
            }

            course_percentage = (lecture_percentage * 50.0f) + (lab_percentage * 50.0f);
        } else {
            course_percentage = lecture_percentage;
        }

        gwa += course_percentage * (course->units / total_units);
    }

    if (gwa > 0.0f) {
        return gwa;
    }

    return 0.0f;
}
