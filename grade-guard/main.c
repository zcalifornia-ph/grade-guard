/*



    CMSC 18 FINAL PROJECT | A GRADE CALCULATOR AND TRACKER | UNIVERSITY OF THE PHILIPPINES MINDANAO

    By: Zildjian E. California - Ravhen M. Grageda - Charisse C. Lorejo - Jhaye Marie H. Gonzales

    Description: Grade Guard is a software tool designed to assist students in monitoring their academic
                 progress on a per-semester basis. It calculates and projects semester grades based on
                 user-defined course parameters, including the weights of various assessment components
                 such as quizzes, exams, and assignments. By inputting achieved scores for completed assessments,
                 students can obtain a clear understanding of their current standing and explore potential outcomes
                 based on hypothetical future performance. This functionality is achieved through a weighted
                 average algorithm, detailed in the following sections. Grade Guard aims to provide students
                 with a simple and effective way to manage their academic performance and stay informed about
                 their progress throughout the semester.



*/

/** --- --- --- Preprocessor Directives --- --- --- **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "header/models.h"
#include "header/ui_console.h"
#include "header/vector.h"

/** --- --- --- Constant Definitions --- --- --- **/

/*

    3.00 Passing
    2.00 Average
    1.75 Cum Laude
    1.45 Magna Cum Laude
    1.20 Summa Cum Laude

*/

const float GRADE_GOALS[] = {3.00, 2.375, 1.75, 1.45, 1.20};

/** --- --- --- Function Prototypes --- --- --- **/

/** --- --- --- Driver Functions --- --- --- **/
void ui_show_profile_header(Student_Profile* profile); // Function to display profile header
void ui_profile_login(Student_Profile* profile); // Function for profile login

Student_Profile create_new_profile(); // Function to create a new student profile
void input_courses(Student_Profile* profile); // Function to input courses for the profile

float percentage_to_gwa(float percentage); // Function to convert percentage to GWA
float calculate_goal_percentage(float predicted_gwa, float goal_gwa); // Function to calculate percentage of goal reached
float calculate_weighted_average(Student_Profile* profile); // Function to calculate the general weighted average

// Function to save Student_Profile to a CSV file
void save_student_profile_to_csv(Student_Profile* profile, char* dir);

// Function to load Student_Profile from a CSV file
Student_Profile load_student_profile_from_csv(char* dir);

// Function to check if a file exists
int file_exists(const char* filename);

char* append_size_to_csv(size_t size);

static void vector_chars_to_cstring(const Vector* source, char* destination)
    {
        size_t length;

        if(destination == NULL)
            {
                return;
            }

        length = vector_size(source);

        if(source != NULL && source->data != NULL && length > 0)
            {
                memcpy(destination, source->data, length);
            }

        destination[length] = '\0';
    }

static char* vector_chars_duplicate(const Vector* source)
    {
        char* duplicate;

        duplicate = malloc(vector_size(source) + 1);
        if(duplicate == NULL)
            {
                return NULL;
            }

        vector_chars_to_cstring(source, duplicate);

        return duplicate;
    }

static const char** option_array_create(size_t count)
    {
        size_t allocation_count;

        allocation_count = count > 0 ? count : 1;

        return (const char**)malloc(sizeof(const char*) * allocation_count);
    }

/** --- --- --- Main Function --- --- --- **/

int main()
    {
        size_t i = 0;
        int selected;

        Vector* student_profiles = vector_create(sizeof(Student_Profile));

        ui_hide_caret();
        ui_fullscreen(); // hide caret, and make screen turn to fullscreen with standard font size
        ui_set_font_size();

        while(1)
            {
                char* dst = append_size_to_csv(i);

                if(file_exists(dst))
                    {
                        Student_Profile profile = load_student_profile_from_csv(dst);
                        vector_push_back(student_profiles, &profile);
                        free(dst);
                    }
                else{
                        free(dst);
                        break;
                    }

                i++;
            }

        while(1)
            {
                /** --- --- --- Main Menu --- --- --- **/

                ui_header();
                selected = ui_selection("Welcome User!",
                                        "[Main Menu]",
                                        UI_LIST_BULLET_POINT, 3,
                                        "New Profile",
                                        "Select Profile",
                                        "Exit");

                switch(selected)
                    {
                        case 0: /// New Profile
                            {
                                Student_Profile buff = create_new_profile();
                                vector_push_back(student_profiles, &buff);

                                break;
                            }

                        case 1: /// Select Profile
                            {
                                ui_header();

                                printf("    [Select Profile]\n\n");

                                Vector* string = vector_create(sizeof(char));
                                char* student_number;

                                    ui_field_input("Enter Student Number", string); // Use the existing input function for course units
                                    printf("\n");

                                    // Convert input from vector to float
                                    student_number = vector_chars_duplicate(string);

                                // Free the unit input vector
                                vector_destroy(string);

                                if(student_number == NULL)
                                    {
                                        ui_show_failure("Unable to allocate student number buffer.");
                                        break;
                                    }

                                Student_Profile* selected_profile = NULL;

                                // Search for the profile by student number
                                for(i = 0; i < vector_size(student_profiles); i++)
                                    {
                                        Student_Profile* profile = (Student_Profile*)vector_at(student_profiles, i);
                                        if(strcmp(profile->student_number, student_number) == 0)
                                            {
                                                selected_profile = profile;
                                                break;
                                            }
                                    }

                                if(selected_profile){ui_clear_screen(); ui_profile_login(selected_profile);}
                                else{ui_show_failure("Profile not found, returning to main menu");}

                                free(student_number);

                                break;
                            }

                        case 2: /// Exit
                            {
                                for(i = 0; i < vector_size(student_profiles); i++)
                                    {
                                        char* dst = append_size_to_csv(i);
                                        save_student_profile_to_csv((Student_Profile*)vector_at(student_profiles,i), dst);
                                        free(dst);
                                    }

                                /*

                                for(i = 0; i < vector_size(student_profiles); i++)
                                    {
                                        student_profile_destroy((Student_Profile*)vector_at(vector_size,i));
                                    }

                                vector_destroy(student_profiles);

                                */

                                return 0;
                            }
                    }
            }

        return 0;
    }

/** --- --- --- File Functions --- --- --- **/

// Function to save Student_Profile to a CSV file
void save_student_profile_to_csv(Student_Profile* profile, char* dir)
    {
        size_t i, j, k;

        if(profile == NULL || dir == NULL)
            {
                return; // Ensure profile and directory are not NULL
            }

        FILE* file = fopen(dir, "w");
        if(!file)
            {
                return; // Failed to open file
            }

        // Write the profile information
        fprintf(file, "First Name,Middle Name,Last Name,Student Number,Degree Program,Predicted GWA,Goal\n");
        fprintf(file, "%s,%s,%s,%s,%s,%.2f,%.2f\n",
                profile->first_name ? profile->first_name : "N/A",
                profile->middle_name ? profile->middle_name : "N/A",
                profile->last_name ? profile->last_name : "N/A",
                profile->student_number ? profile->student_number : "N/A",
                profile->degree_program ? profile->degree_program : "N/A",
                profile->predicted_gwa,
                profile->goal);

        // Write the courses
        fprintf(file, "Courses\n");
        for(i = 0; i < vector_size(profile->courses); i++)
            {
                Course* course = (Course*)vector_at(profile->courses, i);
                if(course == NULL) continue; // Skip if course is NULL

                fprintf(file, "%s,%.2f,%s\n",
                        course->name ? course->name : "N/A",
                        course->units,
                        course->lab_flag ? "Yes" : "No");

                // Write lecture parameters
                fprintf(file, "Lecture Parameters\n");
                for(j = 0; j < vector_size(course->lecture.parameters); j++)
                    {
                        Course_Parameter* param = (Course_Parameter*)vector_at(course->lecture.parameters, j);
                        if (param == NULL) continue; // Skip if param is NULL

                        fprintf(file, "%s,%.2f,%.2f\n",
                                param->parameter_name ? param->parameter_name : "N/A",
                                param->weight,
                                param->percentage);

                        // Write activities
                        for(k = 0; k < vector_size(param->activities); k++)
                            {
                                Activities* activity = (Activities*)vector_at(param->activities, k);
                                if(activity == NULL)continue; // Skip if activity is NULL

                                fprintf(file, "%s,%.2f,%.2f\n",
                                        activity->activity_name ? activity->activity_name : "N/A",
                                        activity->score,
                                        activity->total_score);
                            }
                    }

                // Write lab parameters if they exist
                if(course->lab_flag)
                    {
                        fprintf(file, "Lab Parameters\n");
                        for(j = 0; j < vector_size(course->lab.parameters); j++)
                            {
                                Course_Parameter* param = (Course_Parameter*)vector_at(course->lab.parameters, j);
                                if(param == NULL) continue; // Skip if param is NULL

                                fprintf(file, "%s,%.2f,%.2f\n",
                                        param->parameter_name ? param->parameter_name : "N/A",
                                        param->weight,
                                        param->percentage);

                                // Write activities
                                for(k = 0; k < vector_size(param->activities); k++)
                                    {
                                        Activities* activity = (Activities*)vector_at(param->activities, k);
                                        if(activity == NULL) continue; // Skip if activity is NULL

                                        fprintf(file, "%s,%.2f,%.2f\n",
                                                activity->activity_name ? activity->activity_name : "N/A",
                                                activity->score,
                                                activity->total_score);
                                    }
                            }
                    }
            }

        fclose(file);
    }

// Function to load Student_Profile from a CSV file
Student_Profile load_student_profile_from_csv(char* dir)
    {
        Student_Profile profile;

        if(!student_profile_init(&profile))
            {
                return profile;
            }

        FILE* file = fopen(dir, "r");
        if(!file)
            {
                return profile; // Return empty profile if file cannot be opened
            }

        char line[256];
        // Read the first line (header)
        fgets (line, sizeof(line), file); // Skip header line

        // Read profile information
        fgets(line, sizeof(line), file);
        char* token = strtok(line, ",");
        if (token) profile.first_name = strdup(token);
        token = strtok(NULL, ",");
        if (token) profile.middle_name = strdup(token);
        token = strtok(NULL, ",");
        if (token) profile.last_name = strdup(token);
        token = strtok(NULL, ",");
        if (token) profile.student_number = strdup(token);
        token = strtok(NULL, ",");
        if (token) profile.degree_program = strdup(token);
        token = strtok(NULL, ",");
        if (token) profile.predicted_gwa = atof(token);
        token = strtok(NULL, ",");
        if (token) profile.goal = atof(token);

        // Read courses
        fgets(line, sizeof(line), file); // Skip "Courses" line
        while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
            Course course;
            if(!course_init(&course))
                {
                    fclose(file);
                    return profile;
                }
            course.name = strdup(strtok(line, ","));
            course.units = atof(strtok(NULL, ","));
            course.lab_flag = (strcmp(strtok(NULL, ","), "Yes") == 0);

            // Read lecture parameters
            fgets(line, sizeof(line), file); // Skip "Lecture Parameters" line
            while (fgets(line, sizeof(line), file) && strcmp(line, "Lab Parameters\n") != 0 && strcmp(line, "\n") != 0) {
                Course_Parameter param;
                if(!course_parameter_init(&param))
                    {
                        course_reset(&course);
                        fclose(file);
                        return profile;
                    }
                param.parameter_name = strdup(strtok(line, ","));
                param.weight = atof(strtok(NULL, ","));
                param.percentage = atof(strtok(NULL, ","));

                // Read activities
                while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
                    Activities activity;
                    activities_init(&activity);
                    activity.activity_name = strdup(strtok(line, ","));
                    activity.score = atof(strtok(NULL, ","));
                    activity.total_score = atof(strtok(NULL, ","));
                    vector_push_back(param.activities, &activity);
                }

                vector_push_back(course.lecture.parameters, &param);
            }

            // Read lab parameters if they exist
            if (course.lab_flag) {
                fgets(line, sizeof(line), file); // Skip "Lab Parameters" line
                while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
                    Course_Parameter param;
                    if(!course_parameter_init(&param))
                        {
                            course_reset(&course);
                            fclose(file);
                            return profile;
                        }
                    param.parameter_name = strdup(strtok(line, ","));
                    param.weight = atof(strtok(NULL, ","));
                    param.percentage = atof(strtok(NULL, ","));

                    // Read activities
                    while (fgets(line, sizeof(line), file) && strcmp(line, "\n") != 0) {
                        Activities activity;
                        activities_init(&activity);
                        activity.activity_name = strdup(strtok(line, ","));
                        activity.score = atof(strtok(NULL, ","));
                        activity.total_score = atof(strtok(NULL, ","));
                        vector_push_back(param.activities, &activity);
                    }

                    vector_push_back(course.lab.parameters, &param);
                }
            }

            vector_push_back(profile.courses, &course);
        }

        fclose(file);
        return profile;
    }

int file_exists(const char* filename)
    {
        FILE* file = fopen(filename, "r"); // Try to open the file in read mode
        if(file)
            {
                fclose(file); // Close the file if it was opened successfully
                return 1; // File exists
            }
        return 0; // File does not exist
    }

char* append_size_to_csv(size_t size)
    {
        // Calculate the length of the size_t when converted to a string
        size_t size_length = snprintf(NULL, 0, "%zu", size);

        // Allocate memory for the new string (size + length of ".csv" + null terminator)
        char* result = malloc(size_length + 5);
        if(result == NULL)
            {
                return NULL; // Return NULL if memory allocation fails
            }

        // Create the new string
        snprintf(result, size_length + 1, "%zu", size); // Convert size_t to string
        snprintf(result + size_length, 5, ".csv"); // Append ".csv"

        return result; // Return the new string
    }

void input_courses(Student_Profile* profile)
    {
        char c;
        char* unit_input;
        Vector* string;

        while(true)
            {
                Course new_course;

                if(!course_init(&new_course))
                    {
                        ui_show_failure("Unable to allocate course storage.");
                        break;
                    }

                // Clear the console and redraw the input field
                ui_clear_screen();
                ui_header();
                ui_show_profile_header(profile);

                if(!vector_size(profile->courses)){goto add;}

                printf("    [Current Courses]\n\n");
                printf("    Name\tUnits\tLab\n");
                printf("    -----------------------------\n");

                for(size_t i = 0; i < vector_size(profile->courses); i++)
                    {
                        Course* course = (Course*)vector_at(profile->courses, i);

                        printf("    %s\t%.2f\t%s\n",
                                   course->name ? course->name : "N/A",
                                   course->units,
                                   course->lab_flag ? "Yes" : "No"
                              );
                    }

                printf("\n");

                add:

                printf("    [Add Course]\n\n");

                string = vector_create(sizeof(char));
                ui_field_input("Enter Course Name", string); // Use the existing input function for course name

                    new_course.name = vector_chars_duplicate(string);

                vector_destroy(string);

                if(new_course.name == NULL)
                    {
                        ui_show_failure("Unable to allocate course name.");
                        course_reset(&new_course);
                        break;
                    }

                string = vector_create(sizeof(char));
                    ui_field_input("Enter Course Units", string); // Use the existing input function for course units

                    // Convert input from vector to float
                    unit_input = vector_chars_duplicate(string);

                // Free the unit input vector
                vector_destroy(string);

                if(unit_input == NULL)
                    {
                        ui_show_failure("Unable to allocate course unit buffer.");
                        course_reset(&new_course);
                        break;
                    }

                    new_course.units = atof(unit_input); // Convert to float
                    free(unit_input);

                // Ask if it's a lab course
                printf("\n    Is this a lab course? (Y/N):                      ");
                gotoxy(cursor_x()-21, cursor_y());

                confirmation:

                    c = (char)ui_read_key();
                    if(c == 'y' || c == 'Y')
                        {
                            new_course.lab_flag = true;
                        }
                    else if(c == 'n' || c == 'N'){new_course.lab_flag = false;}
                    else{goto confirmation;}

                    if(new_course.lab_flag){printf("Yes\n\n");}
                    else{printf("No\n\n");}

                // push the new course into the profile's courses vector
                vector_push_back(profile->courses, &new_course);

                // ask if the user wants to add another course
                printf("    Do you want to add another course? (Press ESC to stop, any other key to continue)\n");
                c = (char)ui_read_key();
                if(c == UI_KEY_ESCAPE){ui_clear_screen(); break;}
            }
    }

float percentage_to_gwa(float percentage)
    {
        if(percentage < 0.0){percentage = 0.0;}
        else if(percentage > 1.0){percentage = 1.0;}
        return 1.0 + 4.0 * (1.0 - percentage);
    }

float calculate_goal_percentage(float predicted_gwa, float goal_gwa)
    {
        float normalized_predicted = 6 - predicted_gwa; // Convert predicted GWA
        float normalized_goal = 6 - goal_gwa;           // Convert goal GWA

        // Calculate the percentage of the goal reached
        if (normalized_goal > 0)
            {
                float percentage = (normalized_predicted / normalized_goal) * 100;

                if(percentage > 100){return 100;}
                else if(percentage < 0){return 0;}
                else{return percentage;}
            }
        else
            {
                return 0.0; // Return 0 if the goal is not valid (e.g., goal GWA is 1)
            }
    }

float calculate_weighted_average(Student_Profile* profile)
    {
        float gwa = 0;

        float total_units = 0;

        size_t i, j, k;

        for(i = 0; i < vector_size(profile->courses); i++)
            {
                Course* course = (Course*)vector_at(profile->courses, i);
                total_units += course->units;
            }

        for(i = 0; i < vector_size(profile->courses); i++)
            {
                Course* course = (Course*)vector_at(profile->courses, i);

                float course_percentage = 0.0;
                float lecture_percentage = 0.0;
                float lab_percentage = 0.0;

                // Calculate for Lecture Component
                Course_Component component = course->lecture;

                for (j = 0; j < vector_size(component.parameters); j++)
                    {
                        Course_Parameter* parameter = (Course_Parameter*)vector_at(component.parameters, j);
                        float parameter_total_score = 0.0, parameter_actual_score = 0.0;
                        float parameter_activity_count = vector_size(parameter->activities);

                        // Calculate the total score for the activities in this parameter
                        for (k = 0; k < parameter_activity_count; k++)
                            {
                                Activities* activity = (Activities*)vector_at(parameter->activities, k);
                                if(activity->total_score > 0 && activity->score > 0)
                                    {
                                        parameter_actual_score += activity->score; // Normalize and weight by percentage
                                        parameter_total_score += activity->total_score;
                                    }
                            }

                        // Calculate the parameter's contribution to the lecture's grade
                        if(parameter_activity_count > 0)
                            {
                                float parameter_percentage = parameter_actual_score / parameter_total_score;

                                lecture_percentage += parameter_percentage * (parameter->weight / 100);
                            }
                    }

                if(course->lab_flag)
                    {
                        // Calculate for Lab Component
                        component = course->lab;

                        for (j = 0; j < vector_size(component.parameters); j++)
                            {
                                Course_Parameter* parameter = (Course_Parameter*)vector_at(component.parameters, j);
                                float parameter_total_score = 0.0, parameter_actual_score = 0.0;
                                float parameter_activity_count = vector_size(parameter->activities);

                                // Calculate the total score for the activities in this parameter
                                for (k = 0; k < parameter_activity_count; k++)
                                    {
                                        Activities* activity = (Activities*)vector_at(parameter->activities, k);
                                        if(activity->total_score > 0 && activity->score > 0)
                                            {
                                                parameter_actual_score += activity->score; // Normalize and weight by percentage
                                                parameter_total_score += activity->total_score;
                                            }
                                    }

                                // Calculate the parameter's contribution to the lab's grade
                                if(parameter_activity_count > 0)
                                    {
                                        float parameter_percentage = parameter_actual_score / parameter_total_score;

                                        lab_percentage += parameter_percentage * (parameter->weight / 100);
                                    }
                            }

                        course_percentage = (lecture_percentage * 50) + (lab_percentage * 50);
                    }
                else{course_percentage = lecture_percentage;}

                gwa += course_percentage * ((float)course->units / (float)total_units);
            }

        // Calculate the overall GWA
        if (gwa > 0) {
            return gwa;
        } else {
            return 0.0 ;
        }
    }

void ui_show_profile_header(Student_Profile* profile)
    {
        printf("    [Profile]\n\n");

        printf("    Name: %s %c. %s\n", profile->first_name, profile->middle_name[0], profile->last_name);
        printf("    Student Number: %s\n", profile->student_number);
        printf("    Degree Program: %s\n", profile->degree_program);
        printf("    Grade Goal: %.2f\n\n", profile->goal);

        profile->predicted_gwa = percentage_to_gwa(calculate_weighted_average(profile));

        printf("    Predicted GWA: %.2f\n", profile->predicted_gwa);
        printf("    Percent of Goal: %.2f%%\n\n", calculate_goal_percentage(profile->predicted_gwa, profile->goal));
    }

void ui_profile_login(Student_Profile* profile)
    {
        size_t i;
        int selected;

        Vector* string;

        while(1)
        {
            ui_header();
            ui_show_profile_header(profile);

            selected = ui_selection("[Profile Operations]",
                                    "Select an operation:",
                                    UI_LIST_CAPS_ALPHABETICAL, 5,
                                    "Activity Operations",
                                    "Course Operations",
                                    "Profile Settings",
                                    "View Grades",
                                    "Logout");

            switch(selected)
                {
                    case 0: /// Activity Operations
                        {
                            ui_header();
                            ui_show_profile_header(profile);

                            selected = ui_selection("[Activity Operations]",
                                                    "Select an operation:",
                                                    UI_LIST_LOWS_ALPHABETICAL, 4,
                                                    "Add Activity",
                                                    "Add Score",
                                                    "Delete Activity",
                                                    "Back");

                            switch(selected)
                                {
                                    case 0: /// Add Activity
                                            {
                                                const char** options;
                                                const char** params;
                                                char* total_score_cstr;

                                                ui_header();
                                                ui_show_profile_header(profile);

                                                if(vector_size(profile->courses) == 0)
                                                    {
                                                        ui_show_failure("No courses available.");
                                                        break;
                                                    }

                                                options = option_array_create(vector_size(profile->courses));
                                                if(options == NULL)
                                                    {
                                                        ui_show_failure("Unable to allocate course options.");
                                                        break;
                                                    }

                                                for(i = 0; i < vector_size(profile->courses); i++)
                                                    {
                                                        options[i] = ((Course*)vector_at(profile->courses,i))->name;
                                                    }

                                                selected = ui_selection_array("[Add Activity]", "Select a Course:", UI_LIST_NUMERIC, vector_size(profile->courses), options);
                                                free((void*)options);

                                                Course* course = (Course*)vector_at(profile->courses, selected);
                                                Course_Component* component;

                                                    if(course->lab_flag)
                                                        {
                                                            ui_header();
                                                            ui_show_profile_header(profile);

                                                            component = ui_selection_array("[Add Activity]",
                                                                                           "Select a Component:",
                                                                                           UI_LIST_NUMERIC, 2, (const char* const[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
                                                        }
                                                    else{component = &course->lecture;}

                                                ui_header();
                                                ui_show_profile_header(profile);

                                                if(!vector_size(component->parameters))
                                                    {
                                                        printf("    [Add Activity]\n\n");

                                                        printf("    Cannot add activity as course component parameters do not exist!\n\n");

                                                        ui_show_failure("Failed to add new activity, returning to previous menu");

                                                        break;
                                                    }

                                                params = option_array_create(vector_size(component->parameters));
                                                if(params == NULL)
                                                    {
                                                        ui_show_failure("Unable to allocate parameter options.");
                                                        break;
                                                    }

                                                for(i = 0; i < vector_size(component->parameters); i++)
                                                    {
                                                        params[i] = ((Course_Parameter*)vector_at(component->parameters,i))->parameter_name;
                                                    }

                                                selected = ui_selection_array("[Add Activity]", "Select a Parameter:", UI_LIST_BULLET_POINT, vector_size(component->parameters), params);
                                                free((void*)params);

                                                Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, selected);

                                                ui_header();
                                                ui_show_profile_header(profile);

                                                Activities activity;
                                                activities_init(&activity);

                                                    string = vector_create(sizeof(char)); // allocate a string
                                                        ui_field_input("Enter Activity Name", string);
                                                        activity.activity_name = vector_chars_duplicate(string);

                                                    // Free the input vector
                                                    vector_destroy(string);

                                                    if(activity.activity_name == NULL)
                                                        {
                                                            ui_show_failure("Unable to allocate activity name.");
                                                            break;
                                                        }

                                                    string = vector_create(sizeof(char)); // allocate a string
                                                        ui_field_input("Enter Total Score of Activity", string);

                                                        // Convert input from vector to int
                                                        total_score_cstr = vector_chars_duplicate(string);

                                                    // Free the input vector
                                                    vector_destroy(string);

                                                    if(total_score_cstr == NULL)
                                                        {
                                                            free(activity.activity_name);
                                                            ui_show_failure("Unable to allocate activity score buffer.");
                                                            break;
                                                        }

                                                        activity.score = 0;
                                                        activity.total_score = atoi(total_score_cstr); // Convert to int
                                                        free(total_score_cstr);

                                                vector_push_back(parameter->activities, &activity);

                                                ui_clear_screen();

                                                break;
                                            }

                                    case 1: /// Add Score
                                        {
                                            const char** options;
                                            const char** parameter_options;
                                            const char** activity_options;
                                            char* score_str;

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            if(vector_size(profile->courses) == 0)
                                                {
                                                    ui_show_failure("No courses available.");
                                                    break;
                                                }

                                            options = option_array_create(vector_size(profile->courses));
                                            if(options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate course options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    options[i] = ((Course*)vector_at(profile->courses,i))->name;
                                                }

                                            selected = ui_selection_array("[Add Score]", "Select a Course:", UI_LIST_NUMERIC, vector_size(profile->courses), options);
                                            free((void*)options);

                                            Course* course = (Course*)vector_at(profile->courses, selected);
                                            Course_Component* component;

                                            if(course->lab_flag)
                                                {
                                                    ui_header();
                                                    ui_show_profile_header(profile);

                                                    component = ui_selection_array("[Add Score]",
                                                                                   "Select a Component:",
                                                                                   UI_LIST_NUMERIC, 2, (const char* const[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
                                                }
                                            else
                                                {
                                                    component = &course->lecture;
                                                }

                                            if(!vector_size(component->parameters))
                                                {
                                                    ui_header();
                                                    ui_show_profile_header(profile);

                                                    ui_show_failure("No parameters available for this component.");
                                                    break;
                                                }

                                            // Display parameters and allow user to select one
                                            parameter_options = option_array_create(vector_size(component->parameters));
                                            if(parameter_options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate parameter options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                {
                                                    parameter_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                }

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            selected = ui_selection_array("[Add Score]", "Select a Parameter:", UI_LIST_BULLET_POINT, vector_size(component->parameters), parameter_options);
                                            free((void*)parameter_options);
                                            Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, selected);

                                            // Display activities and allow user to select one to modify the score
                                            if(vector_size(parameter->activities) == 0)
                                                {
                                                    ui_show_failure("No activities available for this parameter.");
                                                    break;
                                                }

                                            activity_options = option_array_create(vector_size(parameter->activities));
                                            if(activity_options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate activity options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(parameter->activities); i++)
                                                {
                                                    activity_options[i] = ((Activities*)vector_at(parameter->activities, i))->activity_name;
                                                }

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            selected = ui_selection_array("[Add Score]", "Select an Activity:", UI_LIST_BULLET_POINT, vector_size(parameter->activities), activity_options);
                                            free((void*)activity_options);
                                            Activities* selected_activity = (Activities*)vector_at(parameter->activities, selected);

                                            // Input new score
                                            ui_header();
                                            ui_show_profile_header(profile);
                                            string = vector_create(sizeof(char));
                                            ui_field_input("Enter New Score", string);

                                            score_str = vector_chars_duplicate(string);

                                            vector_destroy(string);

                                            if(score_str == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate score buffer.");
                                                    break;
                                                }

                                            // Update the score of the selected activity
                                            selected_activity->score = atoi(score_str); // Convert to int and update
                                            free(score_str);

                                            ui_clear_screen();
                                            break;
                                        }

                                    case 2: /// Delete Activity
                                        {
                                            const char** course_options;
                                            const char** activity_options;
                                            const char** activity_names;

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            if(vector_size(profile->courses) == 0)
                                                {
                                                    ui_show_failure("No courses available.");
                                                    break;
                                                }

                                            course_options = option_array_create(vector_size(profile->courses));
                                            if(course_options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate course options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    course_options[i] = ((Course*)vector_at(profile->courses, i))->name;
                                                }

                                            selected = ui_selection_array("[Delete Activity]", "Select a Course:", UI_LIST_NUMERIC, vector_size(profile->courses), course_options);
                                            free((void*)course_options);
                                            Course* course = (Course*)vector_at(profile->courses, selected);
                                            Course_Component* component;

                                            if(course->lab_flag)
                                                {
                                                    ui_header();
                                                    ui_show_profile_header(profile);

                                                    component = ui_selection_array("[Delete Activity]",
                                                                                   "Select a Component:",
                                                                                   UI_LIST_NUMERIC, 2, (const char* const[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
                                                }
                                            else
                                                {
                                                    component = &course->lecture;
                                                }

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            if(!vector_size(component->parameters))
                                                {
                                                    ui_show_failure("No parameters available for this component.");
                                                    break;
                                                }

                                            // Display activities for deletion
                                            activity_options = option_array_create(vector_size(component->parameters));
                                            if(activity_options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate parameter options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                {
                                                    activity_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                }

                                            selected = ui_selection_array("[Delete Activity]", "Select a Parameter:", UI_LIST_BULLET_POINT , vector_size(component->parameters), activity_options);
                                            free((void*)activity_options);
                                            Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, selected);

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            // Display activities for deletion
                                            if(vector_size(parameter->activities) == 0)
                                                {
                                                    ui_show_failure("No activities available for this parameter.");
                                                    break;
                                                }

                                            activity_names = option_array_create(vector_size(parameter->activities));
                                            if(activity_names == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate activity options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(parameter->activities); i++)
                                                {
                                                    activity_names[i] = ((Activities*)vector_at(parameter->activities, i))->activity_name;
                                                }

                                            selected = ui_selection_array("[Delete Activity]", "Select an Activity to Delete:", UI_LIST_BULLET_POINT, vector_size(parameter->activities), activity_names);
                                            free((void*)activity_names);

                                            activities_destroy((Activities*)vector_remove(parameter->activities,selected));

                                            break;
                                        }

                                    case 3: /// Back
                                        break;
                                }

                            break;
                        }

                    case 1: /// Course Operations
                        {
                            ui_header();
                            ui_show_profile_header(profile);

                            printf("    [Current Courses]\n\n");
                            printf("    Name\tUnits\tLab\n");
                            printf("    -----------------------------\n");

                            for(size_t i = 0; i < vector_size(profile->courses); i++)
                                {
                                    Course* course = (Course*)vector_at(profile->courses, i);

                                    printf("    %s\t%.2f\t%s\n",
                                               course->name ? course->name : "N/A",
                                               course->units,
                                               course->lab_flag ? "Yes" : "No"
                                          );
                                }

                            printf("\n");

                            selected = ui_selection("[Course Operations]",
                                                    "Select an operation:",
                                                    UI_LIST_LOWS_ALPHABETICAL, 4,
                                                    "Add Course",
                                                    "Delete Course",
                                                    "Course Parameters",
                                                    "Back");

                            switch(selected)
                                {
                                    case 0: /// Add Course
                                        {
                                            input_courses(profile);
                                            continue;
                                        }

                                    case 1: /// Delete Course
                                        {
                                            const char** course_options;

                                            if(vector_size(profile->courses)<2){break;}

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            course_options = option_array_create(vector_size(profile->courses));
                                            if(course_options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate course options.");
                                                    break;
                                                }

                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    course_options[i] = ((Course*)vector_at(profile->courses, i))->name;
                                                }

                                            selected = ui_selection_array("[Delete Course]", "Select a Course to Delete:", UI_LIST_NUMERIC, vector_size(profile->courses), course_options);
                                            free((void*)course_options);

                                            void* course = vector_remove(profile->courses,selected);
                                            course_destroy(course);

                                            continue;
                                        }

                                    case 2: /// Course Parameters
                                        {
                                            const char** course_options;

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            if(vector_size(profile->courses) == 0)
                                                {
                                                    ui_show_failure("No courses available.");
                                                    continue;
                                                }

                                            course_options = option_array_create(vector_size(profile->courses));
                                            if(course_options == NULL)
                                                {
                                                    ui_show_failure("Unable to allocate course options.");
                                                    continue;
                                                }

                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    course_options[i] = ((Course*)vector_at(profile->courses, i))->name;
                                                }

                                            selected = ui_selection_array("[Set Course Parameters]", "Select a Course:", UI_LIST_NUMERIC, vector_size(profile->courses), course_options);
                                            free((void*)course_options);
                                            Course* course = (Course*)vector_at(profile->courses, selected);
                                            Course_Component* component;

                                               if(course->lab_flag)
                                                    {
                                                        ui_header();
                                                        ui_show_profile_header(profile);

                                                        component = ui_selection_array("[Set Course Parameters]",
                                                                                       "Select a Component:",
                                                                                       UI_LIST_NUMERIC, 2, (const char* const[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
                                                    }
                                                else
                                                    {
                                                        component = &course->lecture;
                                                    }

                                            parameter_operations:

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            int parameter_action = ui_selection("[Course Parameter Operations]",
                                                                                "Select an operation:",
                                                                                UI_LIST_LOWS_ALPHABETICAL, 5,
                                                                                "Add Parameter",
                                                                                "Modify Parameter",
                                                                                "Print Parameters",
                                                                                "Delete Parameter",
                                                                                "Back");

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            switch(parameter_action)
                                                {
                                                    case 0: /// Add Parameter
                                                        {
                                                            char* weight_string;

                                                            Course_Parameter new_param;

                                                            if(!course_parameter_init(&new_param))
                                                                {
                                                                    ui_show_failure("Unable to allocate parameter storage.");
                                                                    goto parameter_operations;
                                                                }

                                                            // Input parameter name
                                                            Vector* name_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter parameter name", name_vector);
                                                            new_param.parameter_name = vector_chars_duplicate(name_vector);
                                                            vector_destroy(name_vector); // Free the temporary vector

                                                            if(new_param.parameter_name == NULL)
                                                                {
                                                                    course_parameter_reset(&new_param);
                                                                    ui_show_failure("Unable to allocate parameter name.");
                                                                    goto parameter_operations;
                                                                }

                                                            // Input weight
                                                            Vector* weight_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter weight (in percent)", weight_vector);
                                                            weight_string = vector_chars_duplicate(weight_vector);
                                                            vector_destroy(weight_vector); // Free the temporary vector

                                                            if(weight_string == NULL)
                                                                {
                                                                    course_parameter_reset(&new_param);
                                                                    ui_show_failure("Unable to allocate parameter weight.");
                                                                    goto parameter_operations;
                                                                }

                                                            new_param.weight = atof(weight_string);
                                                            free(weight_string);

                                                            vector_push_back(component->parameters, &new_param);

                                                            ui_clear_screen();
                                                            goto parameter_operations;
                                                        }

                                                    case 1: /// Modify Parameter
                                                        {
                                                            const char** parameter_options;
                                                            char* new_name;
                                                            char* new_weight_string;

                                                            if(vector_size(component->parameters) == 0)
                                                                {
                                                                    ui_show_failure("No parameters to modify.");
                                                                    goto parameter_operations;
                                                                }

                                                            parameter_options = option_array_create(vector_size(component->parameters));
                                                            if(parameter_options == NULL)
                                                                {
                                                                    ui_show_failure("Unable to allocate parameter options.");
                                                                    goto parameter_operations;
                                                                }

                                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                                {
                                                                    parameter_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                                }

                                                            selected = ui_selection_array("[Modify Parameter]", "Select a Parameter to Modify:", UI_LIST_NUMERIC, vector_size(component->parameters), parameter_options);
                                                            free((void*)parameter_options);
                                                            Course_Parameter* param_to_modify = (Course_Parameter*)vector_at(component->parameters, selected);

                                                            ui_header();
                                                            ui_show_profile_header(profile);

                                                            // Modify parameter name
                                                            Vector* new_name_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter new parameter name (leave blank to keep current)", new_name_vector);

                                                                if(vector_size(new_name_vector) > 0)
                                                                    {
                                                                        new_name = vector_chars_duplicate(new_name_vector);
                                                                        if(new_name == NULL)
                                                                            {
                                                                                vector_destroy(new_name_vector);
                                                                                ui_show_failure("Unable to allocate parameter name.");
                                                                                goto parameter_operations;
                                                                            }

                                                                        free(param_to_modify->parameter_name); // Free old name
                                                                        param_to_modify->parameter_name = new_name;
                                                                    }

                                                            vector_destroy(new_name_vector); // Free the temporary vector

                                                            // Modify weight
                                                            Vector* new_weight_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter new weight (enter -1 to keep current)", new_weight_vector);

                                                                if(vector_size(new_weight_vector) > 0)
                                                                    {
                                                                        new_weight_string = vector_chars_duplicate(new_weight_vector);
                                                                        if(new_weight_string == NULL)
                                                                            {
                                                                                vector_destroy(new_weight_vector);
                                                                                ui_show_failure("Unable to allocate parameter weight.");
                                                                                goto parameter_operations;
                                                                            }
                                                                        float new_weight = atof(new_weight_string);
                                                                        free(new_weight_string);
                                                                        if (new_weight >= 0) { param_to_modify->weight = new_weight; }
                                                                    }

                                                            vector_destroy(new_weight_vector); // Free the temporary vector

                                                            ui_clear_screen();

                                                            goto parameter_operations;
                                                        }

                                                    case 2: /// Print Parameters
                                                        {
                                                            if(vector_size(component->parameters) == 0)
                                                                {
                                                                    ui_show_failure("No parameters to display.");
                                                                    goto parameter_operations;
                                                                }

                                                            printf("    Parameters for Course: %s\n\n", course->name);

                                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                                {
                                                                    Course_Parameter* param = (Course_Parameter*)vector_at(component->parameters, i);
                                                                    printf("        Parameter Name: %s, Weight: %.2f\n",
                                                                           param->parameter_name, param->weight);
                                                                }

                                                            printf("\n    Press any key to continue . . .");
                                                            ui_read_key();

                                                            ui_clear_screen();

                                                            goto parameter_operations;
                                                        }

                                                    case 3: /// Delete Parameter
                                                        {
                                                            const char** parameter_options;

                                                            if(vector_size(component->parameters) == 0)
                                                                {
                                                                    ui_show_failure("No parameters to delete.");
                                                                    goto parameter_operations;
                                                                }

                                                            parameter_options = option_array_create(vector_size(component->parameters));
                                                            if(parameter_options == NULL)
                                                                {
                                                                    ui_show_failure("Unable to allocate parameter options.");
                                                                    goto parameter_operations;
                                                                }

                                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                                {
                                                                    parameter_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                                }

                                                            selected = ui_selection_array("[Delete Parameter]", "Select a Parameter to Delete:", UI_LIST_NUMERIC, vector_size(component->parameters), parameter_options);
                                                            free((void*)parameter_options);
                                                            course_parameter_destroy((Course_Parameter*)vector_remove(component->parameters, selected));
                                                            goto parameter_operations;
                                                        }

                                                    case 4: /// Back
                                                        {
                                                            ui_clear_screen();
                                                            break;
                                                        }
                                                }

                                                continue;
                                            }

                                    case 4: /// Back
                                        break;
                                }

                            continue;
                        }

                    case 2: /// Profile Settings
                    {
                        ui_header();
                        ui_show_profile_header(profile);

                        selected = ui_selection("[Profile Settings]",
                                                "Select a parameter:",
                                                UI_LIST_LOWS_ALPHABETICAL, 2,
                                                "Set Grade Goal",
                                                "Back");

                        switch(selected)
                            {
                                case 0: /// Set Grade Goal
                                    {
                                        char* goal_str;

                                        ui_header();
                                        ui_show_profile_header(profile);
                                        Vector* string = vector_create(sizeof(char));
                                        ui_field_input("Enter New Grade Goal", string);

                                        goal_str = vector_chars_duplicate(string);
                                        vector_destroy(string);

                                        if(goal_str == NULL)
                                            {
                                                ui_show_failure("Unable to allocate goal buffer.");
                                                break;
                                            }

                                        profile->goal = atof(goal_str);
                                        free(goal_str);

                                        ui_clear_screen();
                                        break;
                                    }

                                case 1: /// Back
                                    break;
                            }

                        break;
                    }

                    case 3: /// View Grades
                        {
                            size_t j, k;

                            ui_header();
                            ui_show_profile_header(profile);
                            // Implementation for displaying grades
                            for(i = 0; i < vector_size(profile->courses); i++)
                                {
                                    Course* course = (Course*)vector_at(profile->courses, i);
                                    printf("    Course: %s\n", course->name);

                                    for(j = 0; j < vector_size(course->lecture.parameters); j++)
                                        {
                                            Course_Parameter* param = (Course_Parameter*)vector_at(course->lecture.parameters, j);
                                            printf("      Parameter: %s\n", param->parameter_name);
                                            for(k = 0; k < vector_size(param->activities); k++)
                                                {
                                                    Activities* activity = (Activities*)vector_at(param->activities, k);
                                                    printf("        Activity: %s, Score: %.2f / %.2f\n", activity->activity_name, activity->score, activity->total_score);
                                                }
                                        }

                                    printf("\n");
                                }

                            printf("\n    Press any key to continue . . .");
                            ui_read_key();

                            ui_clear_screen();
                            break;
                        }

                    case 4: /// Logout
                        return;
                }
        }
    }

Student_Profile create_new_profile()
    {
        Student_Profile profile;
        int ch, selected;

        // Loop for confirmation
        while(1)
            {
                if(!student_profile_init(&profile))
                    {
                        return profile;
                    }

                Vector* first_name_vector = vector_create(sizeof(char));
                Vector* middle_name_vector = vector_create(sizeof(char));
                Vector* last_name_vector = vector_create(sizeof(char));
                Vector* student_number_vector = vector_create(sizeof(char));
                Vector* degree_program_vector = vector_create(sizeof(char));

                // Input fields

                ui_header();
                printf("    %s\n\n", "[New Profile]");

                ui_field_input("First Name", first_name_vector);
                ui_field_input("Middle Name", middle_name_vector);
                ui_field_input("Last Name", last_name_vector);
                ui_field_input("Student Number", student_number_vector);
                ui_field_input("Degree Program", degree_program_vector);

                // Allocate memory for profile char* fields

                profile.first_name = malloc(vector_size(first_name_vector) + 1);
                profile.middle_name = malloc(vector_size(middle_name_vector) + 1);
                profile.last_name = malloc(vector_size(last_name_vector) + 1);
                profile.student_number = malloc(vector_size(student_number_vector) + 1);
                profile.degree_program = malloc(vector_size(degree_program_vector) + 1);

                // Convert vectors (C++ strings) to c-strings

                vector_chars_to_cstring(first_name_vector, profile.first_name);

                vector_chars_to_cstring(middle_name_vector, profile.middle_name);

                vector_chars_to_cstring(last_name_vector, profile.last_name);

                vector_chars_to_cstring(student_number_vector, profile.student_number);

                vector_chars_to_cstring(degree_program_vector, profile.degree_program);

                // Free C++-strings
                vector_destroy(first_name_vector);
                vector_destroy(middle_name_vector);
                vector_destroy(last_name_vector);
                vector_destroy(student_number_vector);
                vector_destroy(degree_program_vector);

                // Display the created profile
                printf("\n    Profile Created...                                          \n\n");

                printf("    Is the above information correct? (Press ENTER to confirm, ESC to re-enter)\n");

                profile_confirmation: // Wait for user input

                    ch = ui_read_key();

                    if(ch == UI_KEY_ENTER){break;}
                    else if (ch == UI_KEY_ESCAPE)
                        {
                            ui_clear_screen();

                            // Free allocated memory before restarting
                            student_profile_reset(&profile);

                            continue;
                        }
                    else{goto profile_confirmation;}
            }

        input_courses(&profile);

        get_goal:

        ui_header();
        ui_show_profile_header(&profile);

        selected = ui_selection("[Profile]",
                                "Select Grade Goal:",
                                UI_LIST_NUMERIC, 6,
                                "Passing [3.00]",
                                "Average [2.38]",
                                "Cum Laude [1.75]",
                                "Magna Cum Laude [1.45]",
                                "Summa Cum Laude [1.20]",
                                "Custom");

        if(selected != 5)
            {
                profile.goal = GRADE_GOALS[selected];

                ui_clear_screen();

                ui_header();
                ui_show_profile_header(&profile);

                printf("    Grade Goal: %.2f\n\n", profile.goal);
            }
        else
            {
                char* c_str;

                ui_clear_screen();

                ui_header();
                ui_show_profile_header(&profile);

                Vector* string = vector_create(sizeof(char));
                    ui_field_input("Enter Grade Goal", string); // Use the existing input function for course units
                    printf("\n");

                    // Convert input from vector to float
                    c_str = vector_chars_duplicate(string); // Temporary buffer to hold the string

                // Free the unit input vector
                vector_destroy(string);

                if(c_str == NULL)
                    {
                        ui_show_failure("Unable to allocate goal buffer.");
                        goto get_goal;
                    }

                    profile.goal = atof(c_str); // Convert to float
                    free(c_str);
            }

        printf("    Is the above information correct? (Press ENTER to confirm, ESC to re-enter)\n");

        goal_confirmation: // Wait for user input

            ch = ui_read_key();

            if(ch == UI_KEY_ENTER){ui_clear_screen(); return profile;}
            else if(ch == UI_KEY_ESCAPE)
                {
                    ui_clear_screen();
                    goto get_goal;
                }
            else{goto goal_confirmation;}

        return profile;
    }
