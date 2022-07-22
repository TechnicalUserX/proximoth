#ifndef _PROXIMOTH_DISPLAY_MANAGER_HPP
#define _PROXIMOTH_DISPLAY_MANAGER_HPP

#include "proximoth.hpp"



extern pthread_t proximoth_display_manager_panel_updater_thread;

extern bool proximoth_display_manager_panel_updater_thread_shutdown_request;

void* proximoth_display_manager_panel_updater(void* arg);

void proximoth_display_manager_panel_print(void);

void proximoth_display_set_cursor(int row, int col);

void proximoth_display_manager_panel_print_constants(void);

void proximoth_display_manager_panel_print_elapsed_time(void);

void proximoth_display_manager_panel_print_parameters(void);

#endif