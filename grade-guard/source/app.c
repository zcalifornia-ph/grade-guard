#include "app.h"

#include "persistence.h"
#include "profile_controller.h"
#include "ui_console.h"

#include <stdlib.h>

static void app_initialize_console(void)
{
    ui_hide_caret();
    ui_fullscreen();
    ui_set_font_size();
}

static int app_load_profiles(Vector* student_profiles)
{
    size_t i;

    if (!student_profiles) {
        return 0;
    }

    for (i = 0; ; i++) {
        char* path;

        path = append_size_to_csv(i);
        if (!path) {
            return 0;
        }

        if (!file_exists(path)) {
            free(path);
            break;
        }

        {
            Student_Profile profile = load_student_profile_from_csv(path);
            vector_push_back(student_profiles, &profile);
        }

        free(path);
    }

    return 1;
}

static void app_save_profiles(Vector* student_profiles)
{
    size_t i;

    if (!student_profiles) {
        return;
    }

    for (i = 0; i < vector_size(student_profiles); i++) {
        char* path;

        path = append_size_to_csv(i);
        if (!path) {
            continue;
        }

        save_student_profile_to_csv((Student_Profile*)vector_at(student_profiles, i), path);
        free(path);
    }
}

static void app_destroy_profiles(Vector* student_profiles)
{
    size_t i;

    if (!student_profiles) {
        return;
    }

    for (i = 0; i < vector_size(student_profiles); i++) {
        student_profile_reset((Student_Profile*)vector_at(student_profiles, i));
    }

    vector_destroy(student_profiles);
}

int app_run(void)
{
    int selected;
    Vector* student_profiles;

    student_profiles = vector_create(sizeof(Student_Profile));
    if (!student_profiles) {
        return 1;
    }

    app_initialize_console();

    if (!app_load_profiles(student_profiles)) {
        app_destroy_profiles(student_profiles);
        return 1;
    }

    while (1) {
        ui_header();
        selected = ui_selection(
            "Welcome User!",
            "[Main Menu]",
            UI_LIST_BULLET_POINT,
            3,
            "New Profile",
            "Select Profile",
            "Exit"
        );

        switch (selected) {
            case 0:
            {
                Student_Profile profile = create_new_profile();

                if (!profile.courses) {
                    ui_show_failure("Unable to create a new profile.");
                    break;
                }

                vector_push_back(student_profiles, &profile);
                break;
            }

            case 1:
            {
                Student_Profile* selected_profile;

                selected_profile = profile_select_by_student_number(student_profiles);
                if (selected_profile) {
                    ui_clear_screen();
                    ui_profile_login(selected_profile);
                }

                break;
            }

            case 2:
                app_save_profiles(student_profiles);
                app_destroy_profiles(student_profiles);
                return 0;

            default:
                break;
        }
    }

    return 0;
}
