#include "ui_console.h"

#include <conio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static const char kLowerAlphabet[] = "abcdefghijklmnopqrstuvwxyz";
static const char kUpperAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char kFieldHint[] = "    Press Enter to save or Backspace to delete.";

static HANDLE ui_stdout_handle(void)
{
    HANDLE console_handle;

    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console_handle == NULL || console_handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    return console_handle;
}

static int ui_console_width(HANDLE console_handle)
{
    CONSOLE_SCREEN_BUFFER_INFO screen_info;

    if (!console_handle || !GetConsoleScreenBufferInfo(console_handle, &screen_info)) {
        return 0;
    }

    return (int)screen_info.dwSize.X;
}

static WORD ui_console_attributes(HANDLE console_handle)
{
    CONSOLE_SCREEN_BUFFER_INFO screen_info;

    if (!console_handle || !GetConsoleScreenBufferInfo(console_handle, &screen_info)) {
        return (WORD)((UI_COLOR_BLACK << 4) | UI_COLOR_WHITE);
    }

    return screen_info.wAttributes;
}

static void ui_clear_region(HANDLE console_handle, int x, int y, int length)
{
    COORD coord;
    DWORD written;
    WORD attributes;

    if (!console_handle || x < 0 || y < 0 || length <= 0) {
        return;
    }

    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    attributes = ui_console_attributes(console_handle);

    FillConsoleOutputCharacterA(console_handle, ' ', (DWORD)length, coord, &written);
    FillConsoleOutputAttribute(console_handle, attributes, (DWORD)length, coord, &written);
    SetConsoleCursorPosition(console_handle, coord);
}

static void ui_clear_line_at(int y)
{
    HANDLE console_handle;
    int width;

    console_handle = ui_stdout_handle();
    width = ui_console_width(console_handle);
    ui_clear_region(console_handle, 0, y, width);
}

static void ui_clear_to_line_end(int x, int y)
{
    HANDLE console_handle;
    int width;

    console_handle = ui_stdout_handle();
    width = ui_console_width(console_handle);

    if (width <= x) {
        return;
    }

    ui_clear_region(console_handle, x, y, width - x);
}

static void ui_format_option(char* buffer, size_t buffer_size, UiListMode list_mode, int index, const char* option)
{
    const char* safe_option;

    safe_option = option ? option : "";

    switch (list_mode) {
        case UI_LIST_NUMERIC:
            snprintf(buffer, buffer_size, "%d. %s", index + 1, safe_option);
            break;
        case UI_LIST_CAPS_ALPHABETICAL:
            snprintf(
                buffer,
                buffer_size,
                "%c. %s",
                kUpperAlphabet[(size_t)index % (sizeof(kUpperAlphabet) - 1U)],
                safe_option
            );
            break;
        case UI_LIST_LOWS_ALPHABETICAL:
            snprintf(
                buffer,
                buffer_size,
                "%c. %s",
                kLowerAlphabet[(size_t)index % (sizeof(kLowerAlphabet) - 1U)],
                safe_option
            );
            break;
        case UI_LIST_BULLET_POINT:
            snprintf(buffer, buffer_size, "* %s", safe_option);
            break;
        default:
            snprintf(buffer, buffer_size, "%d. %s", index + 1, safe_option);
            break;
    }
}

static void ui_draw_option_row(int y, UiListMode list_mode, int index, int is_selected, const char* option)
{
    char buffer[512];

    ui_format_option(buffer, sizeof(buffer), list_mode, index, option);
    ui_clear_line_at(y);
    gotoxy(0, y);
    printf("    ");

    if (is_selected) {
        ui_color(UI_COLOR_BLACK, UI_COLOR_WHITE);
    }

    printf("%s", buffer);

    if (is_selected) {
        ui_reset_color();
    }
}

static void ui_render_field_state(const Vector* field_vector, int input_x, int input_y)
{
    size_t i;

    ui_clear_to_line_end(input_x, input_y);
    gotoxy(input_x, input_y);

    for (i = 0; i < vector_size(field_vector); i++) {
        printf("%c", *(char*)vector_at((Vector*)field_vector, i));
    }

    ui_clear_line_at(input_y + 1);
    gotoxy(0, input_y + 1);
    printf("%s", kFieldHint);
    gotoxy(input_x + (int)vector_size(field_vector), input_y);
}

void ui_fullscreen(void)
{
    DWORD console_mode;
    HANDLE console_handle;
    HWND console_window;

    console_window = GetConsoleWindow();
    if (console_window == NULL) {
        return;
    }

    console_handle = ui_stdout_handle();
    if (!console_handle) {
        return;
    }

    console_mode = 0;
    if (!GetConsoleDisplayMode(&console_mode)) {
        return;
    }

    if (!(console_mode & CONSOLE_FULLSCREEN_MODE)) {
        if (!SetConsoleDisplayMode(console_handle, CONSOLE_FULLSCREEN_MODE, NULL)) {
            return;
        }
    }
}

void ui_set_font_size(void)
{
    int character_height;
    int character_width;
    int screen_height;
    int screen_width;
    CONSOLE_FONT_INFOEX font_info;
    CONSOLE_SCREEN_BUFFER_INFO screen_info;
    HANDLE console_handle;

    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);
    console_handle = ui_stdout_handle();
    if (!console_handle) {
        return;
    }

    if (!GetConsoleScreenBufferInfo(console_handle, &screen_info)) {
        return;
    }

    character_width = (float)screen_width /
        (float)(screen_info.srWindow.Right - screen_info.srWindow.Left + 1);
    character_height = (float)screen_height /
        (float)(screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1);

    font_info.cbSize = sizeof(font_info);
    if (!GetCurrentConsoleFontEx(console_handle, FALSE, &font_info)) {
        return;
    }

    font_info.dwFontSize.X = character_width;
    font_info.dwFontSize.Y = character_height;
    SetCurrentConsoleFontEx(console_handle, FALSE, &font_info);
}

void ui_clear_screen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO screen_info;
    COORD home;
    DWORD cell_count;
    DWORD written;
    HANDLE console_handle;

    console_handle = ui_stdout_handle();
    if (!console_handle || !GetConsoleScreenBufferInfo(console_handle, &screen_info)) {
        system("cls");
        return;
    }

    home.X = 0;
    home.Y = 0;
    cell_count = (DWORD)screen_info.dwSize.X * (DWORD)screen_info.dwSize.Y;

    FillConsoleOutputCharacterA(console_handle, ' ', cell_count, home, &written);
    FillConsoleOutputAttribute(console_handle, screen_info.wAttributes, cell_count, home, &written);
    SetConsoleCursorPosition(console_handle, home);
}

int ui_read_key(void)
{
    int key_code;

    key_code = getch();
    if (key_code == 0 || key_code == 224) {
        key_code = getch();
    }

    return key_code;
}

void ui_header(void)
{
    ui_color(UI_COLOR_RED, UI_COLOR_BLACK);

    puts("\n  dP\"\"b8    88\"\"Yb       db       8888b.     888888        dP\"\"b8    88   88       db       88\"\"Yb    8888b.  ");
    puts(" dP   `\"    88__dP      dPYb       8I  Yb    88__         dP   `\"    88   88      dPYb      88__dP     8I  Yb ");
    puts(" Yb  \"88    88\"Yb      dP__Yb      8I  dY    88\"\"         Yb  \"88    Y8   8P     dP__Yb     88\"Yb      8I  dY ");
    puts("  YboodP    88  Yb    dP\"\"\"\"Yb    8888Y\"     888888        YboodP    `YbodP'    dP\"\"\"\"Yb    88  Yb    8888Y\"  ");

    ui_color(UI_COLOR_GREEN, UI_COLOR_BLACK);
    printf("\n       CMSC 18 FINAL PROJECT");

    ui_reset_color();
    printf(" | ");

    ui_color(UI_COLOR_GREEN, UI_COLOR_BLACK);
    printf("A GRADE CALCULATOR AND TRACKER");

    ui_reset_color();
    printf(" | ");

    ui_color(UI_COLOR_GREEN, UI_COLOR_BLACK);
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

int ui_selection(const char* heading, const char* message, UiListMode list_mode, int size, ...)
{
    int i;
    int input;
    int selected;
    int y;
    va_list args;

    selected = 0;
    if (size <= 0) {
        return -1;
    }

    printf("    %s\n\n", heading);
    printf("    %s\n\n", message);

    y = cursor_y();

    while (1) {
        va_start(args, size);
        for (i = 0; i < size; i++) {
            ui_draw_option_row(y + i, list_mode, i, i == selected, va_arg(args, const char*));
        }
        va_end(args);

        gotoxy(0, y + size);
        input = ui_read_key();
        if (ui_selection_handler(input, &selected, UI_SELECTION_BOTH, 0, size) == -2) {
            ui_clear_screen();
            break;
        }
    }

    return selected;
}

int ui_selection_array(const char* heading, const char* message, UiListMode list_mode, int size, const char* const* options)
{
    int i;
    int input;
    int selected;
    int y;

    selected = 0;
    if (size <= 0 || !options) {
        return -1;
    }

    printf("    %s\n\n", heading);
    printf("    %s\n\n", message);

    y = cursor_y();

    while (1) {
        for (i = 0; i < size; i++) {
            ui_draw_option_row(y + i, list_mode, i, i == selected, options[i]);
        }

        gotoxy(0, y + size);
        input = ui_read_key();
        if (ui_selection_handler(input, &selected, UI_SELECTION_BOTH, 0, size) == -2) {
            ui_clear_screen();
            break;
        }
    }

    return selected;
}

int ui_selection_handler(int key_code, int* selected, UiSelectionAxis axis, int lower_bound, int upper_bound)
{
    if (key_code == UI_KEY_ENTER) {
        return -2;
    }

    switch (axis) {
        case UI_SELECTION_VERTICAL:
            if ((key_code == UI_KEY_UP_ARROW || key_code == 'w' || key_code == 'W') &&
                *selected - 1 > lower_bound - 1) {
                (*selected) -= 1;
            } else if ((key_code == UI_KEY_DOWN_ARROW || key_code == 's' || key_code == 'S') &&
                *selected + 1 < upper_bound) {
                (*selected) += 1;
            }
            break;

        case UI_SELECTION_HORIZONTAL:
            if ((key_code == UI_KEY_LEFT_ARROW || key_code == 'a' || key_code == 'A') &&
                *selected - 1 > lower_bound - 1) {
                (*selected) -= 1;
            } else if ((key_code == UI_KEY_RIGHT_ARROW || key_code == 'd' || key_code == 'D') &&
                *selected + 1 < upper_bound) {
                (*selected) += 1;
            }
            break;

        case UI_SELECTION_BOTH:
            if ((key_code == UI_KEY_UP_ARROW || key_code == 'w' || key_code == 'W') &&
                *selected - 1 > lower_bound - 1) {
                (*selected) -= 1;
            } else if ((key_code == UI_KEY_DOWN_ARROW || key_code == 's' || key_code == 'S') &&
                *selected + 1 < upper_bound) {
                (*selected) += 1;
            } else if ((key_code == UI_KEY_LEFT_ARROW || key_code == 'a' || key_code == 'A') &&
                *selected - 1 > lower_bound - 1) {
                (*selected) -= 1;
            } else if ((key_code == UI_KEY_RIGHT_ARROW || key_code == 'd' || key_code == 'D') &&
                *selected + 1 < upper_bound) {
                (*selected) += 1;
            }
            break;

        default:
            return -1;
    }

    return 0;
}

void ui_color(UiColor text_color, UiColor background_color)
{
    HANDLE console_handle;

    console_handle = ui_stdout_handle();
    if (!console_handle) {
        return;
    }

    SetConsoleTextAttribute(console_handle, ((background_color << 4) | text_color));
}

void ui_reset_color(void)
{
    ui_color(UI_COLOR_WHITE, UI_COLOR_BLACK);
}

void ui_hide_caret(void)
{
    CONSOLE_CURSOR_INFO info;
    HANDLE console_handle;

    console_handle = ui_stdout_handle();
    if (!console_handle) {
        return;
    }

    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console_handle, &info);
}

int cursor_x(void)
{
    CONSOLE_SCREEN_BUFFER_INFO screen_info;
    HANDLE console_handle;

    console_handle = ui_stdout_handle();
    if (console_handle && GetConsoleScreenBufferInfo(console_handle, &screen_info)) {
        return screen_info.dwCursorPosition.X;
    }

    return 0;
}

int cursor_y(void)
{
    CONSOLE_SCREEN_BUFFER_INFO screen_info;
    HANDLE console_handle;

    console_handle = ui_stdout_handle();
    if (console_handle && GetConsoleScreenBufferInfo(console_handle, &screen_info)) {
        return screen_info.dwCursorPosition.Y;
    }

    return 0;
}

void gotoxy(int x, int y)
{
    COORD coord;
    HANDLE console_handle;

    console_handle = ui_stdout_handle();
    if (!console_handle) {
        return;
    }

    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(console_handle, coord);
}

void ui_field_input(const char* field_name, Vector* field_vector)
{
    char c;
    int x;
    int y;

    if (!field_name || !field_vector) {
        return;
    }

    printf("    %s: ", field_name);
    x = cursor_x();
    y = cursor_y();
    ui_render_field_state(field_vector, x, y);

    while ((c = (char)ui_read_key()) != UI_KEY_ENTER) {
        if (c == UI_KEY_BACK_SPACE) {
            vector_pop_back(field_vector);
        } else if (isprint((unsigned char)c)) {
            vector_push_back(field_vector, &c);
        }

        ui_render_field_state(field_vector, x, y);
    }

    ui_clear_line_at(y + 1);
    gotoxy(0, y + 1);
}

void ui_show_failure(const char* message)
{
    printf("    %s", message);

    printf(" .");
    Sleep(500);

    printf(" .");
    Sleep(500);

    printf(" .");
    Sleep(500);

    ui_clear_screen();
}
