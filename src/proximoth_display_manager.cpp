#include "../include/proximoth_display_manager.hpp"
#include "../include/proximoth_config.hpp"
#include "../include/proximoth_misc_tools.hpp"
#include "../include/proximoth_frame_sniffer.hpp"
#include "../include/proximoth_rts_injector.hpp"
#include "../include/proximoth_interface_mgmt.hpp"
#include "../include/proximoth_error.hpp"


pthread_t proximoth_display_manager_panel_updater_thread;

bool proximoth_display_manager_panel_updater_thread_shutdown_request = false;

void proximoth_display_manager_panel_print(void){

    struct winsize w;
    if(ioctl(0, TIOCGWINSZ, &w) == -1){
        exit(PROXIMOTH_ERROR_IOCTL);
    }

    if(w.ws_col > 84 and w.ws_row > 14){
        printf(" ╰┤" COLOR RGB(0, 182, 220) "Proximoth" CLEAR "├╯\n"
        " ╭──────────────────────────────────────────────────────────────────────────────────╮\n"
        " │ State:                                                                           │\n"
        " ├──────────────────────────────────────┬───────────────────────────────────────────┤\n"
        " │ BSSID     :                          │  Catch Period(sec) :                      │\n"
        " │ Target    :                          │  Total CTS catched :                      │\n"
        " │ Interface :                          │  Last CTS Time     :                      │\n"
        " ├──────────────────────────────────────┴─────────────────────────┬─────────────────┤\n"
        " │ Injected RTS Frames  :                                         │ Last CTS Signal │\n"
        " │ Total Frames Catched :                                         ├─────────────────┤\n"
        " │ Time elapsed(sec)    :                                         │                 │\n"
        " ╰────────────────────────────────────────────────────────────────┴─────────────────╯\n\n");
    }else if(w.ws_col > 32 and w.ws_row > 12){
        printf(" ╰┤" COLOR RGB(0, 182, 220) "Proximoth" CLEAR "├╯\n"
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
        printf(COLOR RGB(255,0,0) "Low terminal\nresolution\n" CLEAR);
    }
    fflush(stdout);

}

void* proximoth_display_manager_panel_updater(void* arg){

    system("clear");
    proximoth_display_manager_panel_print();

    while(1){
        if(proximoth_display_manager_panel_updater_thread_shutdown_request){
            pthread_exit(NULL);
        }
        
        proximoth_display_manager_panel_print_constants();
        proximoth_display_manager_panel_print_elapsed_time();
        proximoth_display_manager_panel_print_parameters();
        usleep(100000);
    }
}

void proximoth_display_set_cursor(int row, int col){
    printf("\033[%d;%dH",row,col);
    fflush(stdout);
}

void proximoth_display_manager_panel_print_constants(void){

    std::string bssid_mac_string = proximoth_toolset_convert_mac_to_string(proximoth_config_bssid_mac);
    std::string target_mac_string = proximoth_toolset_convert_mac_to_string(proximoth_config_target_mac);

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    if(w.ws_col > 84 and w.ws_row > 14){
        proximoth_display_set_cursor(5,16);
        printf(COLOR BOLD RGB(75, 126, 163) "%s" CLEAR,bssid_mac_string.c_str());

        proximoth_display_set_cursor(6,16);
        printf(COLOR BOLD RGB(123, 174, 212) "%s" CLEAR,target_mac_string.c_str());

        proximoth_display_set_cursor(7,16);
        printf(COLOR BOLD RGB(184, 225, 255) "%-16s" CLEAR COLOR BOLD RGB(184, 225, 255) "(Ch: %d)" CLEAR,proximoth_interface_name,proximoth_interface_channel);

    }else if(w.ws_col > 32 and w.ws_row > 12){

        proximoth_display_set_cursor(5,12);
        printf(COLOR BOLD RGB(123, 174, 212) "%s" CLEAR,target_mac_string.c_str());

    }

    proximoth_display_set_cursor(14,1);
    fflush(stdout);

}

void proximoth_display_manager_panel_print_elapsed_time(void){
    
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    struct timeval current;
    uint64_t elapsed_time = 0;
    gettimeofday(&current,0);
    current.tv_sec += proximoth_config_time_gmt_offset;
    elapsed_time = current.tv_sec - proximoth_config_start_time.tv_sec;

    if(w.ws_col > 84 and w.ws_row > 14){
        proximoth_display_set_cursor(11,27);
        printf(COLOR RGB(250, 172, 187) "%lu" CLEAR,elapsed_time);
        proximoth_display_set_cursor(14,1);
        fflush(stdout);
    }
    
}

void proximoth_display_manager_panel_print_parameters(void){

    uint64_t epoch = proximoth_cts_last_time.tv_sec;
    epoch %= 3600*24;
    int h = 0,m = 0,s = 0;
    s = (epoch -= (m =  (epoch -= (h = epoch/3600)*3600)/60)*60);

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    if(w.ws_col > 84 and w.ws_row > 14){

        proximoth_display_set_cursor(5,64);
        printf(COLOR BOLD RGB(21, 138, 69) "%.6lf" CLEAR,proximoth_cts_catch_period);
        
        proximoth_display_set_cursor(6,64);
        printf(COLOR BOLD RGB(48, 179, 102) "%lu" CLEAR,proximoth_cts_total_catched);
        
        proximoth_display_set_cursor(7,64);
        printf(COLOR BOLD RGB(102, 255, 165) "%02d:%02d:%02d" CLEAR,h,m,s);

        proximoth_display_set_cursor(9,27);
        printf(COLOR BOLD RGB(245, 76, 107) "%lu" CLEAR,proximoth_rts_total_injected);

        proximoth_display_set_cursor(10,27);
        printf(COLOR BOLD RGB(247, 119, 143) "%lu" CLEAR,proximoth_total_frames_catched);
        
        proximoth_display_set_cursor(11,69);
        printf("               ");
        proximoth_display_set_cursor(11,69);

        if(proximoth_cts_last_signal_strength < -90){
            printf(COLOR RGB(255,0,0)   "▂___ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -60){
            printf(COLOR RGB(255,100,0) "▂▄__ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -30){
            printf(COLOR RGB(255,255,0) "▂▄▆_ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else{
            printf(COLOR RGB(0,255,0)   "▂▄▆█ dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }

        proximoth_display_set_cursor(3,11);
        if( time(NULL)+proximoth_config_time_gmt_offset - proximoth_cts_last_time.tv_sec <= 5){
            printf(COLOR BOLD RGB(30, 200, 30) "%-30s" CLEAR,"CTS Catch!");
        }else{
            printf(COLOR RGB(186, 255, 193) "%-30s" CLEAR,"Waiting for any CTS...");
        }

        proximoth_display_set_cursor(14,1);
        fflush(stdout);
    }else if(w.ws_col > 32 and w.ws_row > 12){

        proximoth_display_set_cursor(9,9);
        printf(COLOR BOLD RGB(48, 179, 102) "%lu" CLEAR,proximoth_cts_total_catched);
        
        proximoth_display_set_cursor(7,15);
        printf(COLOR BOLD RGB(102, 255, 165) "%02d:%02d:%02d" CLEAR,h,m,s);

        proximoth_display_set_cursor(8,9);
        printf(COLOR BOLD RGB(245, 76, 107) "%lu" CLEAR,proximoth_rts_total_injected);

        proximoth_display_set_cursor(6,12);
        printf("                 ");
        proximoth_display_set_cursor(6,12);

        if(proximoth_cts_last_signal_strength < -90){
            printf(COLOR RGB(255,0,0)   "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -60){
            printf(COLOR RGB(255,100,0) "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else if(proximoth_cts_last_signal_strength < -30){
            printf(COLOR RGB(255,255,0) "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }else{
            printf(COLOR RGB(0,255,0)   "dBm (%hhd)" CLEAR, proximoth_cts_last_signal_strength);
        }

        proximoth_display_set_cursor(3,4);
        if( time(NULL)+proximoth_config_time_gmt_offset - proximoth_cts_last_time.tv_sec <= 5){
            printf(COLOR BOLD RGB(30, 200, 30) "%-25s" CLEAR,"CTS Catch!");
        }else{
            printf(COLOR RGB(186, 255, 193) "%-25s" CLEAR,"Waiting for any CTS...");
        }

        proximoth_display_set_cursor(11,1);
        fflush(stdout);

    }

}