#ifndef GRADE_GUARD_HEADER_UI_CONSOLE_H
#define GRADE_GUARD_HEADER_UI_CONSOLE_H

#include "vector.h"

typedef enum UiListMode {
    UI_LIST_NUMERIC = 0,
    UI_LIST_LOWS_ALPHABETICAL = 1,
    UI_LIST_CAPS_ALPHABETICAL = 2,
    UI_LIST_BULLET_POINT = 3
} UiListMode;

typedef enum UiSelectionAxis {
    UI_SELECTION_VERTICAL = 0,
    UI_SELECTION_HORIZONTAL = 1,
    UI_SELECTION_BOTH = 2
} UiSelectionAxis;

typedef enum UiKeyCode {
    UI_KEY_LEFT_ARROW = 75,
    UI_KEY_RIGHT_ARROW = 77,
    UI_KEY_UP_ARROW = 72,
    UI_KEY_DOWN_ARROW = 80,
    UI_KEY_ENTER = 13,
    UI_KEY_ESCAPE = 27,
    UI_KEY_BACK_SPACE = 8
} UiKeyCode;

typedef enum UiColor {
    UI_COLOR_BLACK = 0,
    UI_COLOR_BLUE = 1,
    UI_COLOR_GREEN = 2,
    UI_COLOR_AQUA = 3,
    UI_COLOR_RED = 4,
    UI_COLOR_PURPLE = 5,
    UI_COLOR_YELLOW = 6,
    UI_COLOR_WHITE = 7,
    UI_COLOR_GRAY = 8,
    UI_COLOR_LIGHT_BLUE = 9,
    UI_COLOR_LIGHT_GREEN = 10,
    UI_COLOR_LIGHT_AQUA = 11,
    UI_COLOR_LIGHT_RED = 12,
    UI_COLOR_LIGHT_PURPLE = 13,
    UI_COLOR_LIGHT_YELLOW = 14,
    UI_COLOR_BRIGHT_WHITE = 15
} UiColor;

void ui_fullscreen(void);
void ui_set_font_size(void);

void ui_header(void);
void ui_box(const char* heading, const char* message);

int ui_selection(const char* heading, const char* message, UiListMode list_mode, int size, ...);
int ui_selection_array(const char* heading, const char* message, UiListMode list_mode, int size, const char* const* options);
int ui_selection_handler(int key_code, int* selected, UiSelectionAxis axis, int lower_bound, int upper_bound);

void ui_color(UiColor text_color, UiColor background_color);
void ui_reset_color(void);
void ui_hide_caret(void);

int cursor_x(void);
int cursor_y(void);
void gotoxy(int x, int y);

void ui_field_input(const char* field_name, Vector* field_vector);
void ui_show_failure(const char* message);

#endif
