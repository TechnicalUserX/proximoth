#include "../include/proximoth_threads.h"
#include "../include/proximoth_error.h"
#include "../include/proximoth_display_manager.h"
#include "../include/proximoth_frame_sniffer.h"
#include "../include/proximoth_rts_injector.h"

int proximoth_threads_start(void){
    if(pthread_create(&proximoth_display_manager_panel_updater_thread,NULL,proximoth_display_manager_panel_updater,NULL)){
        return PROXIMOTH_ERROR_THREADS_CANNOT_START;
    }

    if(pthread_create(&proximoth_frame_sniffer_thread,NULL,proximoth_frame_sniffer,NULL)){
        return PROXIMOTH_ERROR_THREADS_CANNOT_START;       
    }

    if(pthread_create(&proximoth_rts_injector_thread,NULL,proximoth_rts_injector,NULL)){
        return PROXIMOTH_ERROR_THREADS_CANNOT_START;       
    }

    return PROXIMOTH_SUCCESS;
}

int proximoth_threads_wait(void){

    if(pthread_join(proximoth_display_manager_panel_updater_thread,NULL) != 0){
        return PROXIMOTH_ERROR_THREADS_BAD_JOIN;
    }

    if(pthread_join(proximoth_rts_injector_thread,NULL) != 0){
        return PROXIMOTH_ERROR_THREADS_BAD_JOIN;
    }

    if(pthread_join(proximoth_frame_sniffer_thread,NULL) != 0){
        return PROXIMOTH_ERROR_THREADS_BAD_JOIN;
    }

    return PROXIMOTH_SUCCESS;
}