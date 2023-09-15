#include <proximoth/tui/display.h>
#include <proximoth/utility/config.h>
#include <proximoth/utility/error.h>
#include <proximoth/wireless/mac.h>
#include <proximoth/wireless/cts_sniffer.h>
#include <proximoth/wireless/rts_injector.h>
#include <proximoth/wireless/interface.h>

pthread_t proximoth_display_panel_updater_thread;

sem_t proximoth_display_access;

atomic_bool proximoth_display_panel_updater_thread_shutdown_request = false;

void proximoth_display_set_cursor_position(int row, int col){
    fprintf(stdout,"\033[%d;%dH",row,col);
    fflush(stdout);
}

void proximoth_display_set_cursor_visibility(bool visible){
    if(visible){
        fprintf(stdout,"\033[?25h");
        fflush(stdout);
    }else{
        fprintf(stdout,"\033[?25l");
        fflush(stdout);        
    }
}

void* proximoth_display_panel_updater(void* arg){

    proximoth_display_panel_print();

    while(1){
        if(proximoth_display_panel_updater_thread_shutdown_request){
            pthread_exit(NULL);
        }
        
        proximoth_display_panel_print_constants();
        proximoth_display_panel_print_elapsed_time();
        proximoth_display_panel_print_parameters();
        usleep(100000);
    }
}

void proximoth_display_panel_print(void){
    sem_wait(&proximoth_display_access);
    system("clear");
    struct winsize w = {0};
    if(ioctl(0, TIOCGWINSZ, &w) == -1){
        proximoth_error = PROXIMOTH_ERROR_IOCTL;
        exit((int)PROXIMOTH_ERROR_IOCTL);
    }

    if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_WIDE_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_WIDE_ROWS){
        fprintf(stdout," ╰┤" COLOR RGB(0, 182, 220) "Proximoth" CLEAR "├╯\n"
        " ╭──────────────────────────────────────────────────────────────────────────────────╮\n"
        " │ State:                                                                           │\n"
        " ├──────────────────────────────────────┬───────────────────────────────────────────┤\n"
        " │ BSSID     :                          │  Catch Period(sec) :                      │\n"
        " │ Target    :                          │  Total CTS Catched :                      │\n"
        " │ Interface :                          │  Last CTS Time     :                      │\n"
        " ├──────────────────────────────────────┴─────────────────────────┬─────────────────┤\n"
        " │ Injected RTS Frames  :                                         │ Last CTS Signal │\n"
        " │ Total Frames Catched :                                         ├─────────────────┤\n"
        " │ Time Elapsed(sec)    :                                         │                 │\n"
        " ╰────────────────────────────────────────────────────────────────┴─────────────────╯\n\n");
    }else if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_NARROW_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_NARROW_ROWS){
        fprintf(stdout," ╰┤" COLOR RGB(0, 182, 220) "Proximoth" CLEAR "├╯\n"
        " ╭───────────────────────────╮\n"
        " │                           │\n"
        " ├───────────────────────────┤\n"
        " │ Target:                   │\n"
        " │ Signal:                   │\n"
        " │ Last Time:                │\n"
        " │ RTS:                      │\n"
        " │ CTS:                      │\n"
        " ╰───────────────────────────╯\n");
    }else{
        fprintf(stdout, "Low terminal resolution.\n");
    }
    fflush(stdout);
    sem_post(&proximoth_display_access);

}

void proximoth_display_panel_print_constants(void){

    sem_wait(&proximoth_display_access);

    struct winsize w = {0};
    if(ioctl(0, TIOCGWINSZ, &w) == -1){
        proximoth_error = PROXIMOTH_ERROR_IOCTL;
        exit((int)PROXIMOTH_ERROR_IOCTL);
    }

    if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_WIDE_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_WIDE_ROWS){
    
        proximoth_display_set_cursor_position(5,16);
        fprintf(stdout, COLOR BOLD RGB(75, 126, 163) "%s" CLEAR, proximoth_config_bssid_mac_string);

        proximoth_display_set_cursor_position(6,16);
        fprintf(stdout, COLOR BOLD RGB(123, 174, 212) "%s" CLEAR,proximoth_config_target_mac_string);

        proximoth_display_set_cursor_position(7,16);
        fprintf(stdout, COLOR BOLD RGB(184, 225, 255) "%-16s" CLEAR COLOR BOLD RGB(184, 225, 255) "(Ch: %d)" CLEAR,proximoth_interface_name,proximoth_interface_channel);
    
    }else if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_NARROW_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_NARROW_ROWS){
    
        proximoth_display_set_cursor_position(5,12);
        fprintf(stdout, COLOR BOLD RGB(123, 174, 212) "%s" CLEAR,proximoth_config_target_mac_string);
    
    }

    proximoth_display_set_cursor_position(14,1);
    fflush(stdout);
    sem_post(&proximoth_display_access);

}

void proximoth_display_panel_print_elapsed_time(void){
    
    sem_wait(&proximoth_display_access);

    struct winsize w = {0};
    if(ioctl(0, TIOCGWINSZ, &w) == -1){
        proximoth_error = PROXIMOTH_ERROR_IOCTL;
        exit((int)PROXIMOTH_ERROR_IOCTL);
    }
    struct timeval current = {0};
    uint64_t elapsed_time = 0;
    gettimeofday(&current,0);

    elapsed_time = current.tv_sec - proximoth_config_start_time.tv_sec;

    if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_WIDE_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_WIDE_ROWS){
        proximoth_display_set_cursor_position(11,27);
        fprintf(stdout, COLOR RGB(250, 172, 187) "%lu" CLEAR,elapsed_time);
        proximoth_display_set_cursor_position(14,1);
        fflush(stdout);
    }

    sem_post(&proximoth_display_access);

}

void proximoth_display_panel_print_parameters(void){

    sem_wait(&proximoth_display_access);

    time_t epoch = proximoth_cts_last_time.tv_sec;
    struct tm* brokentime = localtime(&epoch);
    int h = brokentime->tm_hour, m = brokentime->tm_min,s = brokentime->tm_sec;


    struct winsize w = {0};
    if(ioctl(0, TIOCGWINSZ, &w) == -1){
        proximoth_error = PROXIMOTH_ERROR_IOCTL;
        exit((int)PROXIMOTH_ERROR_IOCTL);
    }

    if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_WIDE_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_WIDE_ROWS){

        proximoth_display_set_cursor_position(5,64);
        fprintf(stdout, COLOR BOLD RGB(21, 138, 69) "%.6lf" CLEAR,proximoth_cts_catch_period);
        
        proximoth_display_set_cursor_position(6,64);
        fprintf(stdout, COLOR BOLD RGB(48, 179, 102) "%lu" CLEAR,proximoth_cts_total_catched);
        
        proximoth_display_set_cursor_position(7,64);
        if(proximoth_cts_last_time.tv_sec != 0 && proximoth_cts_last_time.tv_usec != 0){
            fprintf(stdout, COLOR BOLD RGB(102, 255, 165) "%02d:%02d:%02d" CLEAR,h,m,s);
        }else{
            fprintf(stdout, COLOR BOLD RGB(102, 255, 165) "--:--:--" CLEAR);
        }

        proximoth_display_set_cursor_position(9,27);
        fprintf(stdout, COLOR BOLD RGB(245, 76, 107) "%lu" CLEAR,proximoth_rts_total_injected);

        proximoth_display_set_cursor_position(10,27);
        fprintf(stdout, COLOR BOLD RGB(247, 119, 143) "%lu" CLEAR,proximoth_total_frames_catched);

        proximoth_display_set_cursor_position(11,69);
        fprintf(stdout, "               ");
        proximoth_display_set_cursor_position(11,69);

        if(proximoth_cts_last_signal_strength < -90){
            fprintf(stdout, COLOR RGB(255,0,0)   "▂___ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -60){
            fprintf(stdout, COLOR RGB(255,100,0) "▂▄__ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -30){
            fprintf(stdout, COLOR RGB(255,255,0) "▂▄▆_ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else{
            fprintf(stdout, COLOR RGB(0,255,0)   "▂▄▆█ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }

        proximoth_display_set_cursor_position(3,11);
        if( time(NULL) - proximoth_cts_last_time.tv_sec <= 5){
            fprintf(stdout, COLOR BOLD RGB(30, 200, 30) "%-30s" CLEAR,"CTS catched!");
        }else{
            fprintf(stdout, COLOR RGB(186, 255, 193) "%-30s" CLEAR,"Listening for CTS...");
        }

        proximoth_display_set_cursor_position(14,1);
        fflush(stdout);
    }else if(w.ws_col > PROXIMOTH_DISPLAY_VIEW_NARROW_COLS && w.ws_row > PROXIMOTH_DISPLAY_VIEW_NARROW_ROWS){

        proximoth_display_set_cursor_position(9,9);
        fprintf(stdout, COLOR BOLD RGB(48, 179, 102) "%lu" CLEAR,proximoth_cts_total_catched);
        
        proximoth_display_set_cursor_position(7,15);
        fprintf(stdout, COLOR BOLD RGB(102, 255, 165) "%02d:%02d:%02d" CLEAR,h,m,s);

        proximoth_display_set_cursor_position(8,9);
        fprintf(stdout, COLOR BOLD RGB(245, 76, 107) "%lu" CLEAR,proximoth_rts_total_injected);

        proximoth_display_set_cursor_position(6,12);
        fprintf(stdout, "                 ");
        proximoth_display_set_cursor_position(6,12);

        if(proximoth_cts_last_signal_strength < -90){
            fprintf(stdout, COLOR RGB(255,0,0)   "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -60){
            fprintf(stdout, COLOR RGB(255,100,0) "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -30){
            fprintf(stdout, COLOR RGB(255,255,0) "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else{
            fprintf(stdout, COLOR RGB(0,255,0)   "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }

        proximoth_display_set_cursor_position(3,4);
        if( time(NULL) - proximoth_cts_last_time.tv_sec <= 5){
            fprintf(stdout, COLOR BOLD RGB(30, 200, 30) "%-25s" CLEAR,"CTS catched!");
        }else{
            fprintf(stdout, COLOR RGB(186, 255, 193) "%-25s" CLEAR,"Listening for CTS...");
        }

        proximoth_display_set_cursor_position(11,1);
        fflush(stdout);

    }
    sem_post(&proximoth_display_access);

}
