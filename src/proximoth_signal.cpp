#include "../include/proximoth_signal.hpp"
#include "../include/proximoth_display_manager.hpp"
#include "../include/proximoth_config.hpp"
#include "../include/proximoth_error.hpp"
#include "../include/proximoth_rts_injector.hpp"
#include "../include/proximoth_frame_sniffer.hpp"

void proximoth_signal_display_manager_resize_event(int sig){

    if(proximoth_config_finished){
        system("clear");
        proximoth_display_manager_panel_print();

    }


}

void proximoth_signal_program_interrupt(int sig){
    
    if(!proximoth_config_finished){
        exit(PROXIMOTH_SUCCESS);
    }

    if(proximoth_config_finished and proximoth_config_flags.f){

        struct timeval current;
        gettimeofday(&current,0);
        current.tv_sec += proximoth_config_time_gmt_offset;
        double t = (current.tv_sec - proximoth_config_start_time.tv_sec) + ( current.tv_usec*1e-6 - proximoth_config_start_time.tv_usec*1e-6);
        uint64_t epoch = proximoth_cts_last_time.tv_sec;
        uint64_t today = epoch % (3600*24);
        
        int h = 0,m = 0,s = 0;
        s = (today -= (m =  (today -= (h = today/3600)*3600)/60)*60);

        fprintf(proximoth_config_file_out,  "[=============================================================]\n"
                                            " - Total Frames Catched : %lu\n"
                                            " - Total RTS Injected   : %lu\n"
                                            " - Total CTS Catched    : %lu\n"
                                            " - Time elapsed         : %.6lf seconds\n"
                                            " - Last CTS Time        : %02d:%02d:%02d\n"
                                            "[=============================================================]\n",\
                                            proximoth_total_frames_catched,\
                                            proximoth_rts_total_injected,\
                                            proximoth_cts_total_catched,\
                                            t,h,m,s);
        fflush(proximoth_config_file_out);
        fclose(proximoth_config_file_out);
    }

    proximoth_display_manager_panel_updater_thread_shutdown_request = true;
    proximoth_frame_sniffer_thread_shutdown_request = true;
    proximoth_rts_injector_thread_shutdown_request = true;

}
