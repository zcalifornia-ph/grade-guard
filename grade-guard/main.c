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
#include <conio.h>
#include <stdarg.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>

#include "header/models.h"
#include "header/vector.h"

/** --- --- --- Constant Definitions --- --- --- **/

/// Selection List Type

#define NUMERIC 0
#define LOWS_ALPHABETICAL 1
#define CAPS_ALPHABETICAL 2
#define BULLET_POINT 3

/// TUI Input Keys

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER 13
#define ESCAPE 27
#define BACK_SPACE 8

/// TUI Output Color

#define BLACK        0
#define BLUE         1
#define GREEN        2
#define AQUA         3
#define RED          4
#define PURPLE       5
#define YELLOW       6
#define WHITE        7
#define GRAY         8
#define LIGHT_BLUE   9
#define LIGHT_GREEN  10
#define LIGHT_AQUA   11
#define LIGHT_RED    12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

/// TUI Selection Type

#define UI_VERTICAL 0
#define UI_HORIZONTAL 1
#define UI_BOTH 2

const char* lower_alphabet = "abcdefghijklmnopqrtsuvwxyz";
const char* upper_alphabet = "ABCDEFGHIJKLMNOPQRTSUVWXYZ";

/*

    3.00 Passing
    2.00 Average
    1.75 Cum Laude
    1.45 Magna Cum Laude
    1.20 Summa Cum Laude

*/

const float GRADE_GOALS[] = {3.00, 2.375, 1.75, 1.45, 1.20};

typedef unsigned long u_long;

/** --- --- --- Function Prototypes --- --- --- **/

/// UI Functions

void ui_fullscreen();
void ui_set_font_size();

void ui_header();
void ui_box(const char* heading, const char* message);

int ui_selection(const char* heading, const char* message, const int list_mode, const int size, ...);

int ui_field(const char* heading, const char* message, const size_t size, ...);

int ui_selection_handler(const char c, int* const selected, const int TYPE, const int lower_bound, const int upper_bound);

void ui_color(const int text_color, const int background_color);
void ui_reset_color();

void ui_hide_caret();

/// File Functions

const char* fgetline(FILE* file);
u_long fcountlines(const char* dst);

/** --- --- --- Driver Functions --- --- --- **/

int cursor_x(); // Function to get the current X position of the cursor
int cursor_y(); // Function to get the current Y position of the cursor
void gotoxy(int x, int y); // Function to set cursor position

void ui_field_input(const char* field_name, Vector* field_vector); // Function for user input in a field
void ui_show_failure(const char* m); // Function to show failure message
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
                                        BULLET_POINT, 3,
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
                                    ui_field_input("Enter Student Number", string); // Use the existing input function for course units
                                    printf("\n");

                                    // Convert input from vector to float
                                    char student_number[vector_size(string) + 1]; // Temporary buffer to hold the string
                                    memcpy(student_number, (char*)vector_at(string, 0), string->element_size*vector_size(string));
                                    student_number[vector_size(string)] = '\0';

                                // Free the unit input vector
                                vector_destroy(string);

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

                                if(selected_profile){system("cls"); ui_profile_login(selected_profile);}
                                else{ui_show_failure("Profile not found, returning to main menu");}

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

/** --- --- --- TUI Output Functions --- --- --- **/

void ui_set_font_size()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Get the console window size in characters
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(!GetConsoleScreenBufferInfo(hConsole, &csbi)){return;}

    int w = (float)screenWidth / (((float)(csbi.srWindow.Right - csbi.srWindow.Left + 1)));
    int h = (float)screenHeight / (((float)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)));

    // Get the current console font information
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    if(!GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo)){return;}

    // Set the new font size
    fontInfo.dwFontSize.X = w;   // Width of each character in the font
    fontInfo.dwFontSize.Y = h; // Height of each character in the font

    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void ui_fullscreen()
    {
        // Retrieve a handle to the console window
        HWND consoleWindow = GetConsoleWindow();
        if(consoleWindow == NULL){return;}

        // Retrieve a handle to the console screen bufferf
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if(hConsole == INVALID_HANDLE_VALUE){return;}

        // Set the console display to fullscreen
        DWORD consoleMode = 0;
        if(!GetConsoleDisplayMode(&consoleMode)){return;}

        // Check if the console is already in fullscreen
        if(!(consoleMode & CONSOLE_FULLSCREEN_MODE))
            {
                if(!SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, NULL)){return;}
            }
    }

void ui_header()
    {
        ui_color(RED,BLACK);

        puts("\n  dP\"\"b8    88\"\"Yb       db       8888b.     888888        dP\"\"b8    88   88       db       88\"\"Yb    8888b.  ");
        puts(" dP   `\"    88__dP      dPYb       8I  Yb    88__         dP   `\"    88   88      dPYb      88__dP     8I  Yb ");
        puts(" Yb  \"88    88\"Yb      dP__Yb      8I  dY    88\"\"         Yb  \"88    Y8   8P     dP__Yb     88\"Yb      8I  dY ");
        puts("  YboodP    88  Yb    dP\"\"\"\"Yb    8888Y\"     888888        YboodP    `YbodP'    dP\"\"\"\"Yb    88  Yb    8888Y\"  ");

        ui_color(GREEN,BLACK);

        printf("\n       CMSC 18 FINAL PROJECT");

        ui_reset_color();
        printf(" | ");
        ui_color(GREEN,BLACK);

        printf("A GRADE CALCULATOR AND TRACKER");

        ui_reset_color();
        printf(" | ");
        ui_color(GREEN,BLACK);

        printf("UNIVERSITY OF THE PHILIPPINES MINDANAO\n");

        ui_reset_color();

        puts("\n        - Zildjian E. California - Ravhen M. Grageda - Charisse C. Lorejo - Jhaye Marie H. Gonzales -\n");
        puts("===============================================================================================================\n");
    }

void ui_box(const char* heading, const char* message)
    {
        ui_header();

        printf("    %s\n\n", heading);
        printf("    %s\n", message);
    }

int ui_selection(const char* heading, const char* message, const int list_mode, const int size, ...)
    {
        int i, y, selected = 0;

        printf("    %s\n\n", heading);
        printf("    %s\n\n", message);

        va_list args; // Initialize variadic list
        va_start(args, size); // Start list

        y = cursor_y(); // initial position

        // Main loop for selection
        while(1)
            {
                // Move the cursor back to the start of the selection
                gotoxy(0, y);

                // Clear the previous selection display
                for(i = 0; i < size; i++){printf("\033[K");} // ANSI escape code to clear the line

                // Display the selection
                va_start(args, size); // start variadic list

                for(i = 0; i < size; i++)
                    {
                        if(i == selected)
                            {
                                printf("    ");
                                ui_color(BLACK, WHITE);

                                switch(list_mode)
                                    {
                                        case NUMERIC:
                                            printf("%d. %s\n", i + 1, va_arg(args, char*));
                                            break;
                                        case CAPS_ALPHABETICAL:
                                            printf("%c. %s\n", upper_alphabet[i % 27], va_arg(args, char*));
                                            break;
                                        case LOWS_ALPHABETICAL:
                                            printf("%c. %s\n", lower_alphabet[i % 27], va_arg(args, char*));
                                            break;
                                        case BULLET_POINT:
                                            printf("* %s\n", va_arg(args, char*));
                                            break;
                                        default:
                                            printf("%d. %s\n", i + 1, va_arg(args, char*));
                                            break;
                                    }

                                ui_reset_color();
                            }
                        else
                            {
                                switch(list_mode)
                                    {
                                        case NUMERIC:
                                            printf("    %d. %s\n", i + 1, va_arg(args, char*));
                                            break;
                                        case CAPS_ALPHABETICAL:
                                            printf("    %c. %s\n", upper_alphabet[i % 27], va_arg(args, char*));
                                            break;
                                        case LOWS_ALPHABETICAL:
                                            printf("    %c. %s\n", lower_alphabet[i % 27], va_arg(args, char*));
                                            break;
                                        case BULLET_POINT:
                                            printf("    * %s\n", va_arg(args, char*));
                                            break;
                                        default:
                                            printf("    %d. %s\n", i + 1, va_arg(args, char*));
                                            break;
                                    }
                            }
                    }

                va_end(args); // End list

                // Handle user input for selection
                int input = getch();
                if (ui_selection_handler(input, &selected, UI_BOTH, 0, size) == -2) {
                    system("cls");
                    break; // Exit the loop if the selection is confirmed
                }
            }

        return selected;
    }

// much better variant of ui_selection, uses array of c strings with size
int ui_selection_array(const char* heading, const char* message, const int list_mode, const int size, char** options)
    {
        int i, y, selected = 0;

        printf("    %s\n\n", heading);
        printf("    %s\n\n", message);

        y = cursor_y(); // initial position

        // Main loop for selection
        while(1)
            {
                // Move the cursor back to the start of the selection
                gotoxy(0, y);

                // Clear the previous selection display
                for(i = 0; i < size; i++)
                    {
                        printf("\033[K"); // ANSI escape code to clear the line
                    }

                // Display the selection
                for(i = 0; i < size; i++)
                    {
                        if(i == selected)
                            {
                                printf("    ");
                                ui_color(BLACK, WHITE);
                            }
                        else{printf("    ");}

                        switch(list_mode)
                            {
                                case NUMERIC:
                                    printf("%d. %s\n", i + 1, options[i]);
                                    break;
                                case CAPS_ALPHABETICAL:
                                    printf("%c. %s\n", upper_alphabet[i % 27], options[i]);
                                    break;
                                case LOWS_ALPHABETICAL:
                                    printf("%c. %s\n", lower_alphabet[i % 27], options[i]);
                                    break;
                                case BULLET_POINT:
                                    printf("* %s\n", options[i]);
                                    break;
                                default:
                                    printf("%d. %s\n", i + 1, options[i]);
                                    break;
                            }

                        if(i == selected){ui_reset_color();}
                    }

                // Handle user input for selection
                int input = getch();
                if (ui_selection_handler(input, &selected, UI_BOTH, 0, size) == -2) {
                    system("cls");
                    break; // Exit the loop if the selection is confirmed
                }
            }

        return selected;
    }

void ui_color(const int text_color, const int background_color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if(hConsole == INVALID_HANDLE_VALUE){return;}

        SetConsoleTextAttribute(hConsole, ((background_color << 4) | text_color));
    }

void ui_reset_color(){ui_color(WHITE,BLACK);}

void ui_hide_caret()
    {
       HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
       CONSOLE_CURSOR_INFO info;

       info.dwSize = 100;
       info.bVisible = FALSE;
       SetConsoleCursorInfo(consoleHandle, &info);
    }

/** --- --- --- TUI Input Functions --- --- --- **/

int ui_selection_handler(const char c, int* const selected, const int TYPE, const int lower_bound, const int upper_bound)
    {
        if(c == ENTER){return -2;}

        switch(TYPE)
            {
                case UI_VERTICAL:
                    {
                        if((c == UP_ARROW || c == 'w' || c == 'W') && *selected-1 > lower_bound-1){(*selected)-=1;}
                        else if((c == DOWN_ARROW || c == 's' || c == 'S') && *selected+1 < upper_bound){(*selected)+=1;}

                        break;
                    }

                case UI_HORIZONTAL:
                    {
                        if((c == LEFT_ARROW || c == 'a' || c == 'A') && *selected-1 > lower_bound-1){(*selected)-=1;}
                        else if((c == RIGHT_ARROW || c == 'd' || c == 'D') && *selected+1 < upper_bound){(*selected)+=1;}

                        break;
                    }

                case UI_BOTH:
                    {
                        if((c == UP_ARROW || c == 'w' || c == 'W') && *selected-1 > lower_bound-1){(*selected)-=1;}
                        else if((c == DOWN_ARROW || c == 's' || c == 'S') && *selected+1 < upper_bound){(*selected)+=1;}
                        else if((c == LEFT_ARROW || c == 'a' || c == 'A') && *selected-1 > lower_bound-1){(*selected)-=1;}
                        else if((c == RIGHT_ARROW || c == 'd' || c == 'D') && *selected+1 < upper_bound){(*selected)+=1;}
                    }

                default:
                    {
                        return -1;
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

/** --- --- --- Driver Functions --- --- --- **/

// Function to get the current X position of the cursor
int cursor_x()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi; // get console handle and get cursor x position

        if(GetConsoleScreenBufferInfo(hConsole, &csbi)){return csbi.dwCursorPosition.X;}
        else{return 0;}
    }

// Function to get the current Y position of the cursor
int cursor_y()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi; // get console handle and get cursor y position

        if(GetConsoleScreenBufferInfo(hConsole, &csbi)){return csbi.dwCursorPosition.Y;}
        else{return 0;}
    }

// Function to set cursor position
void gotoxy(int x, int y)
    {
        COORD coord;
            coord.X = x;
            coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

void ui_field_input(const char* field_name, Vector* field_vector)
    {
        char c;

        // Initial display
        printf("    %s: ", field_name);

        int x = cursor_x(),
            y = cursor_y();

        // Display the current input
        for (size_t i = 0; i < vector_size(field_vector); i++)
            {
                printf("%c", *(char*)vector_at(field_vector, i));
            }

        printf("\n                                               \n    Press Enter to save or Backspace to delete.\n");

        while((c = getch()) != ENTER)
            {
                if(c == BACK_SPACE)
                    {
                        vector_pop_back(field_vector);
                    }
                else if(isprint(c))
                    {
                        vector_push_back(field_vector, &c);
                    }

                // Move the cursor back to the input field
                gotoxy(x, y); // Set cursor position to the start of the input field

                // Clear the current input line
                for (size_t i = 0; i < vector_size(field_vector); i++)
                    {
                        printf("%c", *(char*)vector_at(field_vector, i));
                    }

                // Print spaces to clear any remaining characters if the input is shorter
                for (size_t i = vector_size(field_vector); i < 100; i++) // Assuming a max length of 100
                    {
                        printf(" "); // Print spaces to clear the line
                    }

                // Move the cursor back to the end of the input
                gotoxy(x, y); // Set cursor position to the start of the input field
                for(size_t i = 0; i < vector_size(field_vector); i++)
                    {
                        printf("%c", *(char*)vector_at(field_vector, i));
                    }

                // Move the cursor to the end of the input
                gotoxy(x + vector_size(field_vector), y); // Set cursor position to the end of the input
                printf("\n\n    Press Enter to save or Backspace to delete.\n");
            }

        gotoxy(x, cursor_y()-3);

        printf("\n");
    }

void input_courses(Student_Profile* profile)
    {
        char c;
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
                system("cls");
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

                    new_course.name = malloc(vector_size(string) + 1);
                    memcpy(new_course.name, (char*)vector_at(string, 0), string->element_size*vector_size(string));
                    new_course.name[vector_size(string)] = '\0';

                vector_destroy(string);

                string = vector_create(sizeof(char));
                    ui_field_input("Enter Course Units", string); // Use the existing input function for course units

                    // Convert input from vector to float
                    char unit_input[vector_size(string) + 1]; // Temporary buffer to hold the string
                    memcpy(unit_input, (char*)vector_at(string, 0), string->element_size*vector_size(string));
                    unit_input[vector_size(string)] = '\0';

                    new_course.units = atof(unit_input); // Convert to float

                // Free the unit input vector
                vector_destroy(string);

                // Ask if it's a lab course
                printf("\n    Is this a lab course? (Y/N):                      ");
                gotoxy(cursor_x()-21, cursor_y());

                confirmation:

                    c = getch();
                    if(c == 'y' || c == 'y')
                        {
                            new_course.lab_flag = true;
                        }
                    else if(c == 'n' || c == 'n'){new_course.lab_flag = false;}
                    else{goto confirmation;}

                    if(new_course.lab_flag){printf("Yes\n\n");}
                    else{printf("No\n\n");}

                // push the new course into the profile's courses vector
                vector_push_back(profile->courses, &new_course);

                // ask if the user wants to add another course
                printf("    Do you want to add another course? (Press ESC to stop, any other key to continue)\n");
                c = getch();
                if(c == ESCAPE){system("cls"); break;}
            }
    }

void ui_show_failure(const char* m)
    {
        printf("    %s", m);

        printf(" .");
        Sleep(500);

        printf(" .");
        Sleep(500);

        printf(" .");
        Sleep(500);

        system("cls");
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
                                    CAPS_ALPHABETICAL, 5,
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
                                                    LOWS_ALPHABETICAL, 4,
                                                    "Add Activity",
                                                    "Add Score",
                                                    "Delete Activity",
                                                    "Back");

                            switch(selected)
                                {
                                    case 0: /// Add Activity
                                            {
                                                ui_header();
                                                ui_show_profile_header(profile);

                                                char* options[vector_size(profile->courses)];

                                                for(i = 0; i < vector_size(profile->courses); i++)
                                                    {
                                                        options[i] = ((Course*)vector_at(profile->courses,i))->name;
                                                    }

                                                selected = ui_selection_array("[Add Activity]", "Select a Course:", NUMERIC, vector_size(profile->courses), options);

                                                Course* course = (Course*)vector_at(profile->courses, selected);
                                                Course_Component* component;

                                                    if(course->lab_flag)
                                                        {
                                                            ui_header();
                                                            ui_show_profile_header(profile);

                                                            component = ui_selection_array("[Add Activity]",
                                                                                           "Select a Component:",
                                                                                           NUMERIC, 2, (char*[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
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

                                                char* params[vector_size(component->parameters)];

                                                for(i = 0; i < vector_size(component->parameters); i++)
                                                    {
                                                        params[i] = ((Course_Parameter*)vector_at(component->parameters,i))->parameter_name;
                                                    }

                                                selected = ui_selection_array("[Add Activity]", "Select a Parameter:", BULLET_POINT, vector_size(component->parameters), params);

                                                Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, selected);

                                                ui_header();
                                                ui_show_profile_header(profile);

                                                Activities activity;
                                                activities_init(&activity);

                                                    string = vector_create(sizeof(char)); // allocate a string
                                                        ui_field_input("Enter Activity Name", string);
                                                        activity.activity_name = (char*)malloc(vector_size(string) + 1);
                                                        memcpy(activity.activity_name, (char*)vector_at(string, 0), string->element_size*vector_size(string));
                                                        activity.activity_name[vector_size(string)] = '\0';

                                                    // Free the input vector
                                                    vector_destroy(string);

                                                    string = vector_create(sizeof(char)); // allocate a string
                                                        ui_field_input("Enter Total Score of Activity", string);

                                                        // Convert input from vector to int
                                                        char total_score_cstr[vector_size(string) + 1]; // Temporary buffer to hold the string
                                                        memcpy(total_score_cstr, (char*)vector_at(string, 0), string->element_size*vector_size(string));
                                                        total_score_cstr[vector_size(string)] = '\0';

                                                        activity.score = 0;
                                                        activity.total_score = atoi(total_score_cstr); // Convert to int

                                                    // Free the input vector
                                                    vector_destroy(string);

                                                vector_push_back(parameter->activities, &activity);

                                                system("cls");

                                                break;
                                            }

                                    case 1: /// Add Score
                                        {
                                            ui_header();
                                            ui_show_profile_header(profile);

                                            char* options[vector_size(profile->courses)];

                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    options[i] = ((Course*)vector_at(profile->courses,i))->name;
                                                }

                                            selected = ui_selection_array("[Add Score]", "Select a Course:", NUMERIC, vector_size(profile->courses), options);

                                            Course* course = (Course*)vector_at(profile->courses, selected);
                                            Course_Component* component;

                                            if(course->lab_flag)
                                                {
                                                    ui_header();
                                                    ui_show_profile_header(profile);

                                                    component = ui_selection_array("[Add Score]",
                                                                                   "Select a Component:",
                                                                                   NUMERIC, 2, (char*[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
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
                                            char* parameter_options[vector_size(component->parameters)];
                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                {
                                                    parameter_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                }

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            selected = ui_selection_array("[Add Score]", "Select a Parameter:", BULLET_POINT, vector_size(component->parameters), parameter_options);
                                            Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, selected);

                                            // Display activities and allow user to select one to modify the score
                                            if(vector_size(parameter->activities) == 0)
                                                {
                                                    ui_show_failure("No activities available for this parameter.");
                                                    break;
                                                }

                                            char* activity_options[vector_size(parameter->activities)];
                                            for(i = 0; i < vector_size(parameter->activities); i++)
                                                {
                                                    activity_options[i] = ((Activities*)vector_at(parameter->activities, i))->activity_name;
                                                }

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            selected = ui_selection_array("[Add Score]", "Select an Activity:", BULLET_POINT, vector_size(parameter->activities), activity_options);
                                            Activities* selected_activity = (Activities*)vector_at(parameter->activities, selected);

                                            // Input new score
                                            ui_header();
                                            ui_show_profile_header(profile);
                                            string = vector_create(sizeof(char));
                                            ui_field_input("Enter New Score", string);

                                            char score_str[vector_size(string) + 1];
                                            memcpy(score_str, (char*)vector_at(string, 0), string->element_size * vector_size(string));
                                            score_str[vector_size(string)] = '\0';

                                            // Update the score of the selected activity
                                            selected_activity->score = atoi(score_str); // Convert to int and update

                                            // Free the input vector
                                            vector_destroy(string);

                                            system("cls");
                                            break;
                                        }

                                    case 2: /// Delete Activity
                                        {
                                            ui_header();
                                            ui_show_profile_header(profile);

                                            char* course_options[vector_size(profile->courses)];
                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    course_options[i] = ((Course*)vector_at(profile->courses, i))->name;
                                                }

                                            selected = ui_selection_array("[Delete Activity]", "Select a Course:", NUMERIC, vector_size(profile->courses), course_options);
                                            Course* course = (Course*)vector_at(profile->courses, selected);
                                            Course_Component* component;

                                            if(course->lab_flag)
                                                {
                                                    ui_header();
                                                    ui_show_profile_header(profile);

                                                    component = ui_selection_array("[Delete Activity]",
                                                                                   "Select a Component:",
                                                                                   NUMERIC, 2, (char*[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
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
                                            char* activity_options[vector_size(component->parameters)];
                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                {
                                                    activity_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                }

                                            selected = ui_selection_array("[Delete Activity]", "Select a Parameter:", BULLET_POINT , vector_size(component->parameters), activity_options);
                                            Course_Parameter* parameter = (Course_Parameter*)vector_at(component->parameters, selected);

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            // Display activities for deletion
                                            char* activity_names[vector_size(parameter->activities)];
                                            for(i = 0; i < vector_size(parameter->activities); i++)
                                                {
                                                    activity_names[i] = ((Activities*)vector_at(parameter->activities, i))->activity_name;
                                                }

                                            selected = ui_selection_array("[Delete Activity]", "Select an Activity to Delete:", BULLET_POINT, vector_size(parameter->activities), activity_names);

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
                                                    LOWS_ALPHABETICAL, 4,
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
                                            if(vector_size(profile->courses)<2){break;}

                                            ui_header();
                                            ui_show_profile_header(profile);

                                            char* course_options[vector_size(profile->courses)];
                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    course_options[i] = ((Course*)vector_at(profile->courses, i))->name;
                                                }

                                            selected = ui_selection_array("[Delete Course]", "Select a Course to Delete:", NUMERIC, vector_size(profile->courses), course_options);

                                            void* course = vector_remove(profile->courses,selected);
                                            course_destroy(course);

                                            continue;
                                        }

                                    case 2: /// Course Parameters
                                        {
                                            ui_header();
                                            ui_show_profile_header(profile);

                                            char* course_options[vector_size(profile->courses)];
                                            for(i = 0; i < vector_size(profile->courses); i++)
                                                {
                                                    course_options[i] = ((Course*)vector_at(profile->courses, i))->name;
                                                }

                                            selected = ui_selection_array("[Set Course Parameters]", "Select a Course:", NUMERIC, vector_size(profile->courses), course_options);
                                            Course* course = (Course*)vector_at(profile->courses, selected);
                                            Course_Component* component;

                                               if(course->lab_flag)
                                                    {
                                                        ui_header();
                                                        ui_show_profile_header(profile);

                                                        component = ui_selection_array("[Set Course Parameters]",
                                                                                       "Select a Component:",
                                                                                       NUMERIC, 2, (char*[]){"Lecture","Laboratory"}) ? &course->lab : &course->lecture;
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
                                                                                LOWS_ALPHABETICAL, 5,
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
                                                            Course_Parameter new_param;

                                                            if(!course_parameter_init(&new_param))
                                                                {
                                                                    ui_show_failure("Unable to allocate parameter storage.");
                                                                    goto parameter_operations;
                                                                }

                                                            // Input parameter name
                                                            Vector* name_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter parameter name", name_vector);
                                                            new_param.parameter_name = (char*)malloc(vector_size(name_vector) + 1);
                                                            memcpy(new_param.parameter_name, name_vector->data, vector_size(name_vector));
                                                            new_param.parameter_name[vector_size(name_vector)] = '\0'; // Null-terminate the string
                                                            vector_destroy(name_vector); // Free the temporary vector

                                                            // Input weight
                                                            Vector* weight_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter weight (in percent)", weight_vector);
                                                            {
                                                                char weight_string[vector_size(weight_vector) + 1];
                                                                memcpy(weight_string, weight_vector->data, vector_size(weight_vector));
                                                                weight_string[vector_size(weight_vector)] = '\0';
                                                                new_param.weight = atof(weight_string);
                                                            }
                                                            vector_destroy(weight_vector); // Free the temporary vector

                                                            vector_push_back(component->parameters, &new_param);

                                                            system("cls");
                                                            goto parameter_operations;
                                                        }

                                                    case 1: /// Modify Parameter
                                                        {
                                                            if(vector_size(component->parameters) == 0)
                                                                {
                                                                    ui_show_failure("No parameters to modify.");
                                                                    goto parameter_operations;
                                                                }

                                                            char* parameter_options[vector_size(component->parameters)];
                                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                                {
                                                                    parameter_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                                }

                                                            selected = ui_selection_array("[Modify Parameter]", "Select a Parameter to Modify:", NUMERIC, vector_size(component->parameters), parameter_options);
                                                            Course_Parameter* param_to_modify = (Course_Parameter*)vector_at(component->parameters, selected);

                                                            ui_header();
                                                            ui_show_profile_header(profile);

                                                            // Modify parameter name
                                                            Vector* new_name_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter new parameter name (leave blank to keep current)", new_name_vector);

                                                                if(vector_size(new_name_vector) > 0)
                                                                    {
                                                                        free(param_to_modify->parameter_name); // Free old name
                                                                        param_to_modify->parameter_name = (char*)malloc(vector_size(new_name_vector) + 1);
                                                                        memcpy(param_to_modify->parameter_name, new_name_vector->data, vector_size(new_name_vector));
                                                                        param_to_modify->parameter_name[vector_size(new_name_vector)] = '\0'; // Null-terminate the string
                                                                    }

                                                            vector_destroy(new_name_vector); // Free the temporary vector

                                                            // Modify weight
                                                            Vector* new_weight_vector = vector_create(sizeof(char));
                                                            ui_field_input("Enter new weight (enter -1 to keep current)", new_weight_vector);

                                                                if(vector_size(new_weight_vector) > 0)
                                                                    {
                                                                        char new_weight_string[vector_size(new_weight_vector) + 1];
                                                                        memcpy(new_weight_string, new_weight_vector->data, vector_size(new_weight_vector));
                                                                        new_weight_string[vector_size(new_weight_vector)] = '\0';
                                                                        float new_weight = atof(new_weight_string);
                                                                        if (new_weight >= 0) { param_to_modify->weight = new_weight; }
                                                                    }

                                                            vector_destroy(new_weight_vector); // Free the temporary vector

                                                            system("cls");

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
                                                            getch();

                                                            system("cls");

                                                            goto parameter_operations;
                                                        }

                                                    case 3: /// Delete Parameter
                                                        {
                                                            if(vector_size(component->parameters) == 0)
                                                                {
                                                                    ui_show_failure("No parameters to delete.");
                                                                    goto parameter_operations;
                                                                }

                                                            char* parameter_options[vector_size(component->parameters)];
                                                            for(i = 0; i < vector_size(component->parameters); i++)
                                                                {
                                                                    parameter_options[i] = ((Course_Parameter*)vector_at(component->parameters, i))->parameter_name;
                                                                }

                                                            selected = ui_selection_array("[Delete Parameter]", "Select a Parameter to Delete:", NUMERIC, vector_size(component->parameters), parameter_options);
                                                            course_parameter_destroy((Course_Parameter*)vector_remove(component->parameters, selected));
                                                            goto parameter_operations;
                                                        }

                                                    case 4: /// Back
                                                        {
                                                            system("cls");
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
                                                LOWS_ALPHABETICAL, 2,
                                                "Set Grade Goal",
                                                "Back");

                        switch(selected)
                            {
                                case 0: /// Set Grade Goal
                                    {
                                        ui_header();
                                        ui_show_profile_header(profile);
                                        Vector* string = vector_create(sizeof(char));
                                        ui_field_input("Enter New Grade Goal", string);

                                        char goal_str[vector_size(string) + 1];
                                        memcpy(goal_str, (char*)vector_at(string, 0), string->element_size * vector_size(string));
                                        goal_str[vector_size(string)] = '\0';

                                        profile->goal = atof(goal_str);
                                        vector_destroy(string);

                                        system("cls");
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
                            getch();

                            system("cls");
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

                memcpy(profile.first_name, (char*)vector_at(first_name_vector, 0), first_name_vector->element_size*vector_size(first_name_vector));
                profile.first_name[vector_size(first_name_vector)] = '\0';

                memcpy(profile.middle_name, (char*)vector_at(middle_name_vector, 0), middle_name_vector->element_size*vector_size(middle_name_vector));
                profile.middle_name[vector_size(middle_name_vector)] = '\0';

                memcpy(profile.last_name, (char*)vector_at(last_name_vector, 0), last_name_vector->element_size*vector_size(last_name_vector));
                profile.last_name[vector_size(last_name_vector)] = '\0';

                memcpy(profile.student_number, (char*)vector_at(student_number_vector, 0), student_number_vector->element_size*vector_size(student_number_vector));
                profile.student_number[vector_size(student_number_vector)] = '\0';

                memcpy(profile.degree_program, (char*)vector_at(degree_program_vector, 0), degree_program_vector->element_size*vector_size(degree_program_vector));
                profile.degree_program[vector_size(degree_program_vector)] = '\0';

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

                    ch = getch();

                    if(ch == ENTER){break;}
                    else if (ch == ESCAPE)
                        {
                            system("cls");

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
                                NUMERIC, 6,
                                "Passing [3.00]",
                                "Average [2.38]",
                                "Cum Laude [1.75]",
                                "Magna Cum Laude [1.45]",
                                "Summa Cum Laude [1.20]",
                                "Custom");

        if(selected != 5)
            {
                profile.goal = GRADE_GOALS[selected];

                system("cls");

                ui_header();
                ui_show_profile_header(&profile);

                printf("    Grade Goal: %.2f\n\n", profile.goal);
            }
        else
            {
                system("cls");

                ui_header();
                ui_show_profile_header(&profile);

                Vector* string = vector_create(sizeof(char));
                    ui_field_input("Enter Grade Goal", string); // Use the existing input function for course units
                    printf("\n");

                    // Convert input from vector to float
                    char c_str[vector_size(string) + 1]; // Temporary buffer to hold the string
                    memcpy(c_str, (char*)vector_at(string, 0), string->element_size*vector_size(string));
                    c_str[vector_size(string)] = '\0';

                    profile.goal = atof(c_str); // Convert to float

                // Free the unit input vector
                vector_destroy(string);
            }

        printf("    Is the above information correct? (Press ENTER to confirm, ESC to re-enter)\n");

        goal_confirmation: // Wait for user input

            ch = getch();

            if(ch == ENTER){system("cls"); return profile;}
            else if(ch == ESCAPE)
                {
                    system("cls");
                    goto get_goal;
                }
            else{goto goal_confirmation;}

        return profile;
    }
