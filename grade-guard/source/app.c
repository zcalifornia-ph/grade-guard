#include "app.h"

#include "persistence.h"
#include "profile_controller.h"
#include "ui_console.h"

#include <stdlib.h>

static int app_push_profile(Vector* student_profiles, const Student_Profile* profile)
{
    size_t previous_size;

    if (!student_profiles || !profile) {
        return 0;
    }

    previous_size = vector_size(student_profiles);
    vector_push_back(student_profiles, profile);
    return vector_size(student_profiles) == previous_size + 1;
}

static void app_initialize_console(void)
{
    ui_hide_caret();
    ui_fullscreen();
    ui_set_font_size();
}

static int app_load_profiles(Vector* student_profiles)
{
    Vector* profile_paths;
    size_t i;

    if (!student_profiles) {
        return 0;
    }

    profile_paths = persistence_list_profile_file_paths(".");
    if (!profile_paths) {
        return 0;
    }

    for (i = 0; i < vector_size(profile_paths); i++) {
        char** path;
        Student_Profile profile;
        PersistenceStatus status;

        path = (char**)vector_at(profile_paths, i);
        if (!path || !*path) {
            persistence_destroy_profile_file_paths(profile_paths);
            return 0;
        }

        status = persistence_load_student_profile(*path, &profile);
        if (status != PERSISTENCE_STATUS_OK) {
            persistence_destroy_profile_file_paths(profile_paths);
            return 0;
        }

        if (!app_push_profile(student_profiles, &profile)) {
            student_profile_reset(&profile);
            persistence_destroy_profile_file_paths(profile_paths);
            return 0;
        }
    }

    persistence_destroy_profile_file_paths(profile_paths);
    return 1;
}

static int app_save_profiles(Vector* student_profiles)
{
    size_t i;

    if (!student_profiles) {
        return 0;
    }

    for (i = 0; i < vector_size(student_profiles); i++) {
        char* path;
        PersistenceStatus status;

        path = persistence_build_profile_path(".", i);
        if (!path) {
            return 0;
        }

        status = persistence_save_student_profile((Student_Profile*)vector_at(student_profiles, i), path);
        free(path);

        if (status != PERSISTENCE_STATUS_OK) {
            return 0;
        }
    }

    return 1;
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
        ui_show_failure("Unable to load saved profiles.");
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
                if (!app_save_profiles(student_profiles)) {
                    ui_show_failure("Unable to save all profiles.");
                    break;
                }

                app_destroy_profiles(student_profiles);
                return 0;

            default:
                break;
        }
    }

    return 0;
}
