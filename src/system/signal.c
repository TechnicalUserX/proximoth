#include <proximoth/tui/display.h>
#include <proximoth/utility/config.h>
#include <proximoth/utility/error.h>
#include <proximoth/system/signal.h>
#include <proximoth/wireless/rts_injector.h>
#include <proximoth/wireless/cts_sniffer.h>

void proximoth_signal_display_resize_event(int sig){

    if(proximoth_config_finished){
        proximoth_display_panel_print();
    }

}

void proximoth_signal_program_interrupt(int sig){
    
    proximoth_display_panel_updater_thread_shutdown_request = true;
    proximoth_cts_sniffer_thread_shutdown_request = true;
    proximoth_rts_injector_thread_shutdown_request = true;

}
