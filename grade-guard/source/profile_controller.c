#include "profile_controller.h"

#include "grade_calc.h"
#include "ui_console.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void vector_chars_to_cstring(const Vector* source, char* destination)
{
    size_t length;

    if (!destination) {
        return;
    }

    length = vector_size(source);
    if (source && source->data && length > 0) {
        memcpy(destination, source->data, length);
    }

    destination[length] = '\0';
}

static char* vector_chars_duplicate(const Vector* source)
{
    char* duplicate;

    duplicate = (char*)malloc(vector_size(source) + 1);
    if (!duplicate) {
        return NULL;
    }

    vector_chars_to_cstring(source, duplicate);
    return duplicate;
}

static char* controller_read_text_input(const char* field_name)
{
    Vector* string;
    char* value;

    string = vector_create(sizeof(char));
    if (!string) {
        return NULL;
    }

    ui_field_input(field_name, string);
    value = vector_chars_duplicate(string);
    vector_destroy(string);
    return value;
}

static const char** option_array_create(size_t count)
{
    size_t allocation_count;

    allocation_count = count > 0 ? count : 1;
    return (const char**)malloc(sizeof(const char*) * allocation_count);
}

static void controller_render_profile_screen(Student_Profile* profile)
{
    ui_header();
    ui_show_profile_header(profile);
}

static int controller_wait_for_enter_or_escape(void)
{
    while (1) {
        int key_code = ui_read_key();

        if (key_code == UI_KEY_ENTER || key_code == UI_KEY_ESCAPE) {
            return key_code;
        }
    }
}

static int controller_prompt_yes_no(const char* prompt)
{
    printf("\n    %s (Y/N):                      ", prompt);
    gotoxy(cursor_x() - 21, cursor_y());

    while (1) {
        int key_code = ui_read_key();

        if (key_code == 'y' || key_code == 'Y') {
            printf("Yes\n\n");
            return 1;
        }

        if (key_code == 'n' || key_code == 'N') {
            printf("No\n\n");
            return 0;
        }
    }
}

static void controller_print_course_table(const Student_Profile* profile)
{
    size_t i;

    printf("    [Current Courses]\n\n");
    if (!profile || !profile->courses || vector_size(profile->courses) == 0) {
        printf("    No courses available.\n\n");
        return;
    }

    printf("    Name\tUnits\tLab\n");
    printf("    -----------------------------\n");

    for (i = 0; i < vector_size(profile->courses); i++) {
        Course* course = (Course*)vector_at(profile->courses, i);

        printf(
            "    %s\t%.2f\t%s\n",
            course && course->name ? course->name : "N/A",
            course ? course->units : 0.0f,
            course && course->lab_flag ? "Yes" : "No"
        );
    }

    printf("\n");
}

static int controller_select_course_index(Student_Profile* profile, const char* heading, const char* message)
{
    const char** options;
    size_t i;
    int selected;

    if (!profile || !profile->courses || vector_size(profile->courses) == 0) {
        ui_show_failure("No courses available.");
        return -1;
    }

    options = option_array_create(vector_size(profile->courses));
    if (!options) {
        ui_show_failure("Unable to allocate course options.");
        return -1;
    }

    for (i = 0; i < vector_size(profile->courses); i++) {
        Course* course = (Course*)vector_at(profile->courses, i);

        options[i] = course && course->name ? course->name : "N/A";
    }

    controller_render_profile_screen(profile);
    selected = ui_selection_array(
        heading,
        message,
        UI_LIST_NUMERIC,
        (int)vector_size(profile->courses),
        options
    );
    free((void*)options);
    return selected;
}

static Course_Component* controller_select_component(Student_Profile* profile, Course* course, const char* heading)
{
    int selected;

    if (!course) {
        return NULL;
    }

    if (!course->lab_flag) {
        return &course->lecture;
    }

    controller_render_profile_screen(profile);
    selected = ui_selection_array(
        heading,
        "Select a Component:",
        UI_LIST_NUMERIC,
        2,
        (const char* const[]){"Lecture", "Laboratory"}
    );

    if (selected == 1) {
        return &course->lab;
    }

    return &course->lecture;
}

static int controller_select_parameter_index(
    Student_Profile* profile,
    Course_Component* component,
    const char* heading,
    const char* empty_message
)
{
    const char** options;
    size_t i;
    int selected;

    if (!component || !component->parameters || vector_size(component->parameters) == 0) {
        ui_show_failure(empty_message);
        return -1;
    }

    options = option_array_create(vector_size(component->parameters));
    if (!options) {
        ui_show_failure("Unable to allocate parameter options.");
        return -1;
    }

    for (i = 0; i < vector_size(component->parameters); i++) {
        Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, i);

        options[i] = parameter && parameter->parameter_name ? parameter->parameter_name : "N/A";
    }

    controller_render_profile_screen(profile);
    selected = ui_selection_array(
        heading,
        "Select a Parameter:",
        UI_LIST_BULLET_POINT,
        (int)vector_size(component->parameters),
        options
    );
    free((void*)options);
    return selected;
}

static int controller_select_activity_index(
    Student_Profile* profile,
    Course_Parameter* parameter,
    const char* heading,
    const char* empty_message
)
{
    const char** options;
    size_t i;
    int selected;

    if (!parameter || !parameter->activities || vector_size(parameter->activities) == 0) {
        ui_show_failure(empty_message);
        return -1;
    }

    options = option_array_create(vector_size(parameter->activities));
    if (!options) {
        ui_show_failure("Unable to allocate activity options.");
        return -1;
    }

    for (i = 0; i < vector_size(parameter->activities); i++) {
        Activities* activity = (Activities*)vector_at(parameter->activities, i);

        options[i] = activity && activity->activity_name ? activity->activity_name : "N/A";
    }

    controller_render_profile_screen(profile);
    selected = ui_selection_array(
        heading,
        "Select an Activity:",
        UI_LIST_BULLET_POINT,
        (int)vector_size(parameter->activities),
        options
    );
    free((void*)options);
    return selected;
}

static int controller_capture_profile_identity(Student_Profile* profile)
{
    profile->first_name = controller_read_text_input("First Name");
    if (!profile->first_name) {
        return 0;
    }

    profile->middle_name = controller_read_text_input("Middle Name");
    if (!profile->middle_name) {
        return 0;
    }

    profile->last_name = controller_read_text_input("Last Name");
    if (!profile->last_name) {
        return 0;
    }

    profile->student_number = controller_read_text_input("Student Number");
    if (!profile->student_number) {
        return 0;
    }

    profile->degree_program = controller_read_text_input("Degree Program");
    return profile->degree_program != NULL;
}

static int controller_add_course(Student_Profile* profile)
{
    Course new_course;
    char* unit_input;

    if (!course_init(&new_course)) {
        ui_show_failure("Unable to allocate course storage.");
        return 0;
    }

    printf("    [Add Course]\n\n");
    new_course.name = controller_read_text_input("Enter Course Name");
    if (!new_course.name) {
        course_reset(&new_course);
        ui_show_failure("Unable to allocate course name.");
        return 0;
    }

    unit_input = controller_read_text_input("Enter Course Units");
    if (!unit_input) {
        course_reset(&new_course);
        ui_show_failure("Unable to allocate course unit buffer.");
        return 0;
    }

    new_course.units = (float)atof(unit_input);
    free(unit_input);
    new_course.lab_flag = controller_prompt_yes_no("Is this a lab course");

    vector_push_back(profile->courses, &new_course);
    return 1;
}

static void controller_add_activity(Student_Profile* profile)
{
    int course_index;
    int parameter_index;
    Course* course;
    Course_Component* component;
    Course_Parameter* parameter;
    Activities activity;
    char* total_score_text;

    course_index = controller_select_course_index(profile, "[Add Activity]", "Select a Course:");
    if (course_index < 0) {
        return;
    }

    course = (Course*)vector_at(profile->courses, (size_t)course_index);
    component = controller_select_component(profile, course, "[Add Activity]");
    if (!component) {
        return;
    }

    parameter_index = controller_select_parameter_index(
        profile,
        component,
        "[Add Activity]",
        "No parameters available for this component."
    );
    if (parameter_index < 0) {
        return;
    }

    parameter = (Course_Parameter*)vector_at(component->parameters, (size_t)parameter_index);
    if (!activities_init(&activity)) {
        ui_show_failure("Unable to allocate activity storage.");
        return;
    }

    controller_render_profile_screen(profile);
    activity.activity_name = controller_read_text_input("Enter Activity Name");
    if (!activity.activity_name) {
        activities_reset(&activity);
        ui_show_failure("Unable to allocate activity name.");
        return;
    }

    total_score_text = controller_read_text_input("Enter Total Score of Activity");
    if (!total_score_text) {
        activities_reset(&activity);
        ui_show_failure("Unable to allocate activity score buffer.");
        return;
    }

    activity.score = 0.0f;
    activity.total_score = (float)atof(total_score_text);
    free(total_score_text);

    vector_push_back(parameter->activities, &activity);
    ui_clear_screen();
}

static void controller_add_score(Student_Profile* profile)
{
    int course_index;
    int parameter_index;
    int activity_index;
    Course* course;
    Course_Component* component;
    Course_Parameter* parameter;
    Activities* activity;
    char* score_text;

    course_index = controller_select_course_index(profile, "[Add Score]", "Select a Course:");
    if (course_index < 0) {
        return;
    }

    course = (Course*)vector_at(profile->courses, (size_t)course_index);
    component = controller_select_component(profile, course, "[Add Score]");
    if (!component) {
        return;
    }

    parameter_index = controller_select_parameter_index(
        profile,
        component,
        "[Add Score]",
        "No parameters available for this component."
    );
    if (parameter_index < 0) {
        return;
    }

    parameter = (Course_Parameter*)vector_at(component->parameters, (size_t)parameter_index);
    activity_index = controller_select_activity_index(
        profile,
        parameter,
        "[Add Score]",
        "No activities available for this parameter."
    );
    if (activity_index < 0) {
        return;
    }

    activity = (Activities*)vector_at(parameter->activities, (size_t)activity_index);
    controller_render_profile_screen(profile);
    score_text = controller_read_text_input("Enter New Score");
    if (!score_text) {
        ui_show_failure("Unable to allocate score buffer.");
        return;
    }

    activity->score = (float)atof(score_text);
    free(score_text);
    ui_clear_screen();
}

static void controller_delete_activity(Student_Profile* profile)
{
    int course_index;
    int parameter_index;
    int activity_index;
    Course* course;
    Course_Component* component;
    Course_Parameter* parameter;
    Activities* removed_activity;

    course_index = controller_select_course_index(profile, "[Delete Activity]", "Select a Course:");
    if (course_index < 0) {
        return;
    }

    course = (Course*)vector_at(profile->courses, (size_t)course_index);
    component = controller_select_component(profile, course, "[Delete Activity]");
    if (!component) {
        return;
    }

    parameter_index = controller_select_parameter_index(
        profile,
        component,
        "[Delete Activity]",
        "No parameters available for this component."
    );
    if (parameter_index < 0) {
        return;
    }

    parameter = (Course_Parameter*)vector_at(component->parameters, (size_t)parameter_index);
    activity_index = controller_select_activity_index(
        profile,
        parameter,
        "[Delete Activity]",
        "No activities available for this parameter."
    );
    if (activity_index < 0) {
        return;
    }

    removed_activity = (Activities*)vector_remove(parameter->activities, (size_t)activity_index);
    activities_destroy(removed_activity);
    ui_clear_screen();
}

static void controller_run_activity_operations(Student_Profile* profile)
{
    while (1) {
        int selected;

        controller_render_profile_screen(profile);
        selected = ui_selection(
            "[Activity Operations]",
            "Select an operation:",
            UI_LIST_LOWS_ALPHABETICAL,
            4,
            "Add Activity",
            "Add Score",
            "Delete Activity",
            "Back"
        );

        switch (selected) {
            case 0:
                controller_add_activity(profile);
                break;

            case 1:
                controller_add_score(profile);
                break;

            case 2:
                controller_delete_activity(profile);
                break;

            case 3:
                ui_clear_screen();
                return;

            default:
                break;
        }
    }
}

static void controller_add_parameter(Student_Profile* profile, Course_Component* component)
{
    Course_Parameter new_parameter;
    char* weight_text;

    if (!course_parameter_init(&new_parameter)) {
        ui_show_failure("Unable to allocate parameter storage.");
        return;
    }

    controller_render_profile_screen(profile);
    new_parameter.parameter_name = controller_read_text_input("Enter parameter name");
    if (!new_parameter.parameter_name) {
        course_parameter_reset(&new_parameter);
        ui_show_failure("Unable to allocate parameter name.");
        return;
    }

    weight_text = controller_read_text_input("Enter weight (in percent)");
    if (!weight_text) {
        course_parameter_reset(&new_parameter);
        ui_show_failure("Unable to allocate parameter weight.");
        return;
    }

    new_parameter.weight = (float)atof(weight_text);
    free(weight_text);

    vector_push_back(component->parameters, &new_parameter);
    ui_clear_screen();
}

static void controller_modify_parameter(Student_Profile* profile, Course_Component* component)
{
    int parameter_index;
    Course_Parameter* parameter;
    char* new_name;
    char* new_weight_text;

    parameter_index = controller_select_parameter_index(
        profile,
        component,
        "[Modify Parameter]",
        "No parameters to modify."
    );
    if (parameter_index < 0) {
        return;
    }

    parameter = (Course_Parameter*)vector_at(component->parameters, (size_t)parameter_index);
    controller_render_profile_screen(profile);

    new_name = controller_read_text_input("Enter new parameter name (leave blank to keep current)");
    if (!new_name) {
        ui_show_failure("Unable to allocate parameter name.");
        return;
    }

    if (strlen(new_name) > 0) {
        free(parameter->parameter_name);
        parameter->parameter_name = new_name;
    } else {
        free(new_name);
    }

    new_weight_text = controller_read_text_input("Enter new weight (enter -1 to keep current)");
    if (!new_weight_text) {
        ui_show_failure("Unable to allocate parameter weight.");
        return;
    }

    if (strlen(new_weight_text) > 0) {
        float new_weight = (float)atof(new_weight_text);

        if (new_weight >= 0.0f) {
            parameter->weight = new_weight;
        }
    }

    free(new_weight_text);
    ui_clear_screen();
}

static void controller_print_parameters(Student_Profile* profile, Course* course, Course_Component* component)
{
    size_t i;

    if (!component || !component->parameters || vector_size(component->parameters) == 0) {
        ui_show_failure("No parameters to display.");
        return;
    }

    controller_render_profile_screen(profile);
    printf("    Parameters for Course: %s\n\n", course && course->name ? course->name : "N/A");

    for (i = 0; i < vector_size(component->parameters); i++) {
        Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, i);

        printf(
            "        Parameter Name: %s, Weight: %.2f\n",
            parameter && parameter->parameter_name ? parameter->parameter_name : "N/A",
            parameter ? parameter->weight : 0.0f
        );
    }

    printf("\n    Press any key to continue . . .");
    ui_read_key();
    ui_clear_screen();
}

static void controller_delete_parameter(Student_Profile* profile, Course_Component* component)
{
    int parameter_index;
    Course_Parameter* removed_parameter;

    parameter_index = controller_select_parameter_index(
        profile,
        component,
        "[Delete Parameter]",
        "No parameters to delete."
    );
    if (parameter_index < 0) {
        return;
    }

    removed_parameter = (Course_Parameter*)vector_remove(component->parameters, (size_t)parameter_index);
    course_parameter_destroy(removed_parameter);
    ui_clear_screen();
}

static void controller_run_parameter_operations(Student_Profile* profile, Course* course, Course_Component* component)
{
    while (1) {
        int selected;

        controller_render_profile_screen(profile);
        selected = ui_selection(
            "[Course Parameter Operations]",
            "Select an operation:",
            UI_LIST_LOWS_ALPHABETICAL,
            5,
            "Add Parameter",
            "Modify Parameter",
            "Print Parameters",
            "Delete Parameter",
            "Back"
        );

        switch (selected) {
            case 0:
                controller_add_parameter(profile, component);
                break;

            case 1:
                controller_modify_parameter(profile, component);
                break;

            case 2:
                controller_print_parameters(profile, course, component);
                break;

            case 3:
                controller_delete_parameter(profile, component);
                break;

            case 4:
                ui_clear_screen();
                return;

            default:
                break;
        }
    }
}

static void controller_delete_course(Student_Profile* profile)
{
    int course_index;
    Course* removed_course;

    if (!profile || !profile->courses || vector_size(profile->courses) < 2) {
        ui_show_failure("At least one course must remain on the profile.");
        return;
    }

    course_index = controller_select_course_index(profile, "[Delete Course]", "Select a Course to Delete:");
    if (course_index < 0) {
        return;
    }

    removed_course = (Course*)vector_remove(profile->courses, (size_t)course_index);
    course_destroy(removed_course);
    ui_clear_screen();
}

static void controller_manage_course_parameters(Student_Profile* profile)
{
    int course_index;
    Course* course;
    Course_Component* component;

    course_index = controller_select_course_index(profile, "[Set Course Parameters]", "Select a Course:");
    if (course_index < 0) {
        return;
    }

    course = (Course*)vector_at(profile->courses, (size_t)course_index);
    component = controller_select_component(profile, course, "[Set Course Parameters]");
    if (!component) {
        return;
    }

    controller_run_parameter_operations(profile, course, component);
}

static void controller_run_course_operations(Student_Profile* profile)
{
    while (1) {
        int selected;

        controller_render_profile_screen(profile);
        controller_print_course_table(profile);
        selected = ui_selection(
            "[Course Operations]",
            "Select an operation:",
            UI_LIST_LOWS_ALPHABETICAL,
            4,
            "Add Course",
            "Delete Course",
            "Course Parameters",
            "Back"
        );

        switch (selected) {
            case 0:
                input_courses(profile);
                break;

            case 1:
                controller_delete_course(profile);
                break;

            case 2:
                controller_manage_course_parameters(profile);
                break;

            case 3:
                ui_clear_screen();
                return;

            default:
                break;
        }
    }
}

static void controller_run_profile_settings(Student_Profile* profile)
{
    while (1) {
        int selected;

        controller_render_profile_screen(profile);
        selected = ui_selection(
            "[Profile Settings]",
            "Select a parameter:",
            UI_LIST_LOWS_ALPHABETICAL,
            2,
            "Set Grade Goal",
            "Back"
        );

        switch (selected) {
            case 0:
            {
                char* goal_text;

                controller_render_profile_screen(profile);
                goal_text = controller_read_text_input("Enter New Grade Goal");
                if (!goal_text) {
                    ui_show_failure("Unable to allocate goal buffer.");
                    break;
                }

                profile->goal = (float)atof(goal_text);
                free(goal_text);
                ui_clear_screen();
                break;
            }

            case 1:
                ui_clear_screen();
                return;

            default:
                break;
        }
    }
}

static void controller_print_component_grades(const char* label, Course_Component* component)
{
    size_t i;
    size_t j;

    printf("      %s\n", label);
    if (!component || !component->parameters || vector_size(component->parameters) == 0) {
        printf("        No parameters available.\n");
        return;
    }

    for (i = 0; i < vector_size(component->parameters); i++) {
        Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, i);

        printf("        Parameter: %s\n", parameter && parameter->parameter_name ? parameter->parameter_name : "N/A");
        for (j = 0; parameter && j < vector_size(parameter->activities); j++) {
            Activities* activity = (Activities*)vector_at(parameter->activities, j);

            printf(
                "          Activity: %s, Score: %.2f / %.2f\n",
                activity && activity->activity_name ? activity->activity_name : "N/A",
                activity ? activity->score : 0.0f,
                activity ? activity->total_score : 0.0f
            );
        }
    }
}

static void controller_view_grades(Student_Profile* profile)
{
    size_t i;

    controller_render_profile_screen(profile);
    if (!profile || !profile->courses || vector_size(profile->courses) == 0) {
        printf("    No courses available.\n");
    }

    for (i = 0; profile && profile->courses && i < vector_size(profile->courses); i++) {
        Course* course = (Course*)vector_at(profile->courses, i);

        printf("    Course: %s\n", course && course->name ? course->name : "N/A");
        controller_print_component_grades("Lecture", course ? &course->lecture : NULL);

        if (course && course->lab_flag) {
            controller_print_component_grades("Laboratory", &course->lab);
        }

        printf("\n");
    }

    printf("\n    Press any key to continue . . .");
    ui_read_key();
    ui_clear_screen();
}

void input_courses(Student_Profile* profile)
{
    if (!profile || !profile->courses) {
        return;
    }

    while (1) {
        int key_code;

        ui_clear_screen();
        controller_render_profile_screen(profile);
        controller_print_course_table(profile);

        if (!controller_add_course(profile)) {
            break;
        }

        printf("    Do you want to add another course? (Press ESC to stop, any other key to continue)\n");
        key_code = ui_read_key();
        if (key_code == UI_KEY_ESCAPE) {
            ui_clear_screen();
            break;
        }
    }
}

void ui_show_profile_header(Student_Profile* profile)
{
    if (!profile) {
        return;
    }

    printf("    [Profile]\n\n");

    if (profile->middle_name && profile->middle_name[0] != '\0') {
        printf(
            "    Name: %s %c. %s\n",
            profile->first_name ? profile->first_name : "N/A",
            profile->middle_name[0],
            profile->last_name ? profile->last_name : "N/A"
        );
    } else {
        printf(
            "    Name: %s %s\n",
            profile->first_name ? profile->first_name : "N/A",
            profile->last_name ? profile->last_name : "N/A"
        );
    }

    printf("    Student Number: %s\n", profile->student_number ? profile->student_number : "N/A");
    printf("    Degree Program: %s\n", profile->degree_program ? profile->degree_program : "N/A");
    printf("    Grade Goal: %.2f\n\n", profile->goal);

    profile->predicted_gwa = percentage_to_gwa(calculate_weighted_average(profile));

    printf("    Predicted GWA: %.2f\n", profile->predicted_gwa);
    printf(
        "    Percent of Goal: %.2f%%\n\n",
        calculate_goal_percentage(profile->predicted_gwa, profile->goal)
    );
}

Student_Profile* profile_select_by_student_number(Vector* student_profiles)
{
    char* student_number;
    size_t i;

    if (!student_profiles || vector_size(student_profiles) == 0) {
        ui_show_failure("No profiles available.");
        return NULL;
    }

    ui_header();
    printf("    [Select Profile]\n\n");

    student_number = controller_read_text_input("Enter Student Number");
    if (!student_number) {
        ui_show_failure("Unable to allocate student number buffer.");
        return NULL;
    }

    for (i = 0; i < vector_size(student_profiles); i++) {
        Student_Profile* profile = (Student_Profile*)vector_at(student_profiles, i);

        if (profile && profile->student_number && strcmp(profile->student_number, student_number) == 0) {
            free(student_number);
            return profile;
        }
    }

    free(student_number);
    ui_show_failure("Profile not found, returning to main menu");
    return NULL;
}

void ui_profile_login(Student_Profile* profile)
{
    while (1) {
        int selected;

        controller_render_profile_screen(profile);
        selected = ui_selection(
            "[Profile Operations]",
            "Select an operation:",
            UI_LIST_CAPS_ALPHABETICAL,
            5,
            "Activity Operations",
            "Course Operations",
            "Profile Settings",
            "View Grades",
            "Logout"
        );

        switch (selected) {
            case 0:
                controller_run_activity_operations(profile);
                break;

            case 1:
                controller_run_course_operations(profile);
                break;

            case 2:
                controller_run_profile_settings(profile);
                break;

            case 3:
                controller_view_grades(profile);
                break;

            case 4:
                return;

            default:
                break;
        }
    }
}

Student_Profile create_new_profile(void)
{
    Student_Profile profile = {0};
    int selected;

    while (1) {
        if (!student_profile_init(&profile)) {
            return profile;
        }

        ui_header();
        printf("    [New Profile]\n\n");

        if (!controller_capture_profile_identity(&profile)) {
            student_profile_reset(&profile);
            ui_show_failure("Unable to allocate profile fields.");
            return profile;
        }

        printf("\n    Profile Created...\n\n");
        printf("    Is the above information correct? (Press ENTER to confirm, ESC to re-enter)\n");

        if (controller_wait_for_enter_or_escape() == UI_KEY_ENTER) {
            break;
        }

        ui_clear_screen();
        student_profile_reset(&profile);
    }

    input_courses(&profile);

    while (1) {
        controller_render_profile_screen(&profile);
        selected = ui_selection(
            "[Profile]",
            "Select Grade Goal:",
            UI_LIST_NUMERIC,
            6,
            "Passing [3.00]",
            "Average [2.38]",
            "Cum Laude [1.75]",
            "Magna Cum Laude [1.45]",
            "Summa Cum Laude [1.20]",
            "Custom"
        );

        if (selected >= 0 && selected < GRADE_GUARD_GRADE_GOAL_COUNT) {
            profile.goal = GRADE_GOALS[selected];

            ui_clear_screen();
            controller_render_profile_screen(&profile);
            printf("    Grade Goal: %.2f\n\n", profile.goal);
        } else if (selected == 5) {
            char* goal_text;

            ui_clear_screen();
            controller_render_profile_screen(&profile);
            goal_text = controller_read_text_input("Enter Grade Goal");
            if (!goal_text) {
                ui_show_failure("Unable to allocate goal buffer.");
                continue;
            }

            profile.goal = (float)atof(goal_text);
            free(goal_text);
        } else {
            continue;
        }

        printf("    Is the above information correct? (Press ENTER to confirm, ESC to re-enter)\n");
        if (controller_wait_for_enter_or_escape() == UI_KEY_ENTER) {
            ui_clear_screen();
            return profile;
        }

        ui_clear_screen();
    }

    return profile;
}
