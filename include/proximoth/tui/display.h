#ifndef PROXIMOTH_DISPLAY_MANAGER_H
#define PROXIMOTH_DISPLAY_MANAGER_H

#define PROXIMOTH_DISPLAY_VIEW_WIDE_ROWS 13
#define PROXIMOTH_DISPLAY_VIEW_WIDE_COLS 84

#define PROXIMOTH_DISPLAY_VIEW_NARROW_ROWS 12
#define PROXIMOTH_DISPLAY_VIEW_NARROW_COLS 32

#include <proximoth/proximoth.h>

extern pthread_t proximoth_display_panel_updater_thread;

extern sem_t proximoth_display_access;

extern atomic_bool proximoth_display_panel_updater_thread_shutdown_request;

void proximoth_display_set_cursor_position(int row, int col);

void proximoth_display_set_cursor_visibility(bool visible);

void* proximoth_display_panel_updater(void* arg);

void proximoth_display_panel_print(void);

void proximoth_display_panel_print_constants(void);

void proximoth_display_panel_print_elapsed_time(void);

void proximoth_display_panel_print_parameters(void);

#define COLOR "\033["
#define BOLD "1;"
#define CLEAR "\033[0m"
#define RGB(r,g,b) "38;2;"#r";"#g";"#b"m"

#endif