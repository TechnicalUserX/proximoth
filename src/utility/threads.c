#include <proximoth/utility/threads.h>
#include <proximoth/utility/error.h>
#include <proximoth/utility/config.h>
#include <proximoth/tui/display.h>
#include <proximoth/wireless/cts_sniffer.h>
#include <proximoth/wireless/rts_injector.h>

void proximoth_threads_start(void){

    if(proximoth_config_flags.t == false){
        if(pthread_create(&proximoth_display_panel_updater_thread,NULL,proximoth_display_panel_updater,NULL)){
            proximoth_error = PROXIMOTH_ERROR_THREADS_CANNOT_START;
            exit((int)PROXIMOTH_ERROR_THREADS_CANNOT_START);
        }
    }

    if(pthread_create(&proximoth_cts_sniffer_thread,NULL,proximoth_cts_sniffer,NULL)){
        proximoth_error = PROXIMOTH_ERROR_THREADS_CANNOT_START;
        exit((int)PROXIMOTH_ERROR_THREADS_CANNOT_START);      
    }

    if(pthread_create(&proximoth_rts_injector_thread,NULL,proximoth_rts_injector,NULL)){
        proximoth_error = PROXIMOTH_ERROR_THREADS_CANNOT_START;
        exit((int)PROXIMOTH_ERROR_THREADS_CANNOT_START);      
    }
}

void proximoth_threads_wait(void){

    if(proximoth_config_flags.t == false){
        if(pthread_join(proximoth_display_panel_updater_thread,NULL) != 0){
            proximoth_error = PROXIMOTH_ERROR_THREADS_BAD_JOIN;
            exit((int)PROXIMOTH_ERROR_THREADS_BAD_JOIN); 
        }
    }


    if(pthread_join(proximoth_cts_sniffer_thread,NULL) != 0){
        proximoth_error = PROXIMOTH_ERROR_THREADS_BAD_JOIN;
        exit((int)PROXIMOTH_ERROR_THREADS_BAD_JOIN); 
    }

    if(pthread_join(proximoth_rts_injector_thread,NULL) != 0){
        proximoth_error = PROXIMOTH_ERROR_THREADS_BAD_JOIN;
        exit((int)PROXIMOTH_ERROR_THREADS_BAD_JOIN); 
    }

}
