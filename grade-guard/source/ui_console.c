#include "ui_console.h"

void ui_fullscreen(void)
{
}

void ui_set_font_size(void)
{
}

void ui_header(void)
{
}

void ui_box(const char* heading, const char* message)
{
    (void)heading;
    (void)message;
}

int ui_selection(const char* heading, const char* message, UiListMode list_mode, int size, ...)
{
    (void)heading;
    (void)message;
    (void)list_mode;
    (void)size;
    return 0;
}

int ui_selection_array(const char* heading, const char* message, UiListMode list_mode, int size, const char* const* options)
{
    (void)heading;
    (void)message;
    (void)list_mode;
    (void)size;
    (void)options;
    return 0;
}

int ui_selection_handler(int key_code, int* selected, UiSelectionAxis axis, int lower_bound, int upper_bound)
{
    (void)key_code;
    (void)selected;
    (void)axis;
    (void)lower_bound;
    (void)upper_bound;
    return 0;
}

void ui_color(UiColor text_color, UiColor background_color)
{
    (void)text_color;
    (void)background_color;
}

void ui_reset_color(void)
{
}

void ui_hide_caret(void)
{
}

int cursor_x(void)
{
    return 0;
}

int cursor_y(void)
{
    return 0;
}

void gotoxy(int x, int y)
{
    (void)x;
    (void)y;
}

void ui_field_input(const char* field_name, Vector* field_vector)
{
    (void)field_name;
    (void)field_vector;
}

void ui_show_failure(const char* message)
{
    (void)message;
}
