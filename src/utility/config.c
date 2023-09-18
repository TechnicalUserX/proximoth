#include <proximoth/wireless/mac.h>
#include <proximoth/wireless/interface.h>
#include <proximoth/wireless/radiotap.h>
#include <proximoth/wireless/cts_sniffer.h>
#include <proximoth/wireless/rts_injector.h>
#include <proximoth/tui/display.h>
#include <proximoth/system/signal.h>
#include <proximoth/utility/config.h>
#include <proximoth/utility/error.h>
#include <proximoth/version/version.h>

char proximoth_config_dump_file[PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN+1] = {0};

pcap_dumper_t* proximoth_config_dump_handle = NULL;

struct timeval proximoth_config_start_time = {0};

byte_t proximoth_config_bssid_mac[6] = {0};

byte_t proximoth_config_target_mac[6] = {0};

char proximoth_config_bssid_mac_string[PROXIMOTH_MAC_STRING_SIZE] = {0};

char proximoth_config_target_mac_string[PROXIMOTH_MAC_STRING_SIZE] = {0};

FILE* proximoth_config_file_out = NULL;

struct proximoth_config_flags_t proximoth_config_flags = {false};

atomic_bool proximoth_config_finished = false;

void proximoth_config_print_usage(void){

    fprintf(stdout, "Usage: proximoth [options] <target>\n\n"

                    " <target>                            : MAC address of the target.\n\n"

                    "options:\n\n"

                    " -h, --help                          : Prints this screen.\n\n"

                    " -o <file>, --out-file <file>        : File to write statistics after shutdown.\n\n"
                    
                    " -b <bssid>, --bssid <bssid>         : Custom BSSID to be injected as sender MAC address.\n"
                    "                                       Address is fixed automatically to be global and unicast.\n\n"
                    
                    " -a, --no-mac-autofix                : Disables unicast/global auto fix for BSSID MAC addresses.\n\n"

                    " -i <iface>, --interface <iface>     : Wireless interface to use packet injection and sniffing.\n"
                    "                                       Obligatory option.\n\n"

                    " -d <file>, --dump-file <file>       : Write all CTS captures to a PCAP file.\n\n"

                    " -r <us>, --rts-interval <us>        : Microseconds as threshold to wait between RTS injections.\n"
                    "                                       Setting it to a low value might cause malfunction.\n"
                    "                                       Default: 500000\n\n"

                    " -t, --text-mode                     : Enables text only mode.\n\n"

                    " --version                           : Prints version number and author information.\n\n");

            fflush(stdout);
}

void proximoth_config_print_version(void){

    fprintf(stdout, "Proximoth - Written by TechnicalUserX\n"
                    "Version %s\n",PROXIMOTH_VERSION);
}

void proximoth_config(int argc, char* argv[]){

    gettimeofday(&proximoth_config_start_time, NULL);

    if(atexit(proximoth_config_reset) != 0){
        proximoth_error = PROXIMOTH_ERROR_ATEXIT;
        exit((int)PROXIMOTH_ERROR_ATEXIT);
    }


    static const struct option long_options[] = {
        { "help",           no_argument,        NULL,   'h'},    
        { "interface",      required_argument,  NULL,   'i'},
        { "out-file",       required_argument,  NULL,   'o'},
        { "bssid",          required_argument,  NULL,   'b'},
        { "no-mac-autofix", no_argument,        NULL,   'a'},
        { "rts-interval",   required_argument,  NULL,   'r'},
        { "dump-file",      required_argument,  NULL,   'd'},
        { "text-mode",      no_argument,        NULL,   't'},
        { "version",        no_argument,        NULL,   1000},
        { NULL,             0,                  NULL,   0   }   
    };

    if(setlocale(LC_ALL, setlocale(LC_ALL,"")) == NULL){
        proximoth_error = PROXIMOTH_ERROR_LOCALE;
        exit((int)PROXIMOTH_ERROR_LOCALE);
    }

    srandom(time(NULL));

    if((proximoth_interface_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
        proximoth_error = PROXIMOTH_ERROR_SOCKET_CANNOT_CREATE;
        exit((int)PROXIMOTH_ERROR_SOCKET_CANNOT_CREATE);
    }
    
    if(sem_init(&proximoth_display_access,false,1) != 0){
        proximoth_error = PROXIMOTH_ERROR_SEMAPHORE_CANNOT_INIT;
        exit((int)PROXIMOTH_ERROR_SEMAPHORE_CANNOT_INIT);
    }

    proximoth_radiotap_initialize_align_size(proximoth_radiotap_namespaces);

    struct sigaction proximoth_signal_action_interrupt = {0};
    proximoth_signal_action_interrupt.sa_handler = proximoth_signal_program_interrupt;
    
    if(sigaction(SIGINT,&proximoth_signal_action_interrupt,NULL) == -1){
        proximoth_error = PROXIMOTH_ERROR_SIGNAL_BAD_INIT;
        exit((int)PROXIMOTH_ERROR_SIGNAL_BAD_INIT);
    }

    optind = 0;
    int opt = 0;
    while((opt = getopt_long(argc, argv, ":i:o:b:r:d:hat",long_options,NULL)) != -1){

        switch(opt){

            case 1000:{
                proximoth_config_print_version();
                proximoth_error = PROXIMOTH_SUCCESS;
                exit((int)PROXIMOTH_SUCCESS);
            }
            break;

            case 't':{
                proximoth_config_flags.t = true;
            }
            break;

            case 'a':{
                proximoth_config_flags.a = true;
            }
            break;


            case 'd':{
                strncpy(proximoth_config_dump_file,optarg,PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN);
                proximoth_config_flags.d = true;
            }
            break;


            case 'r':{ // Negative values are not accepted

                char rts_interval_string[21] = {0};
                strncpy(rts_interval_string,optarg,20);

                for(size_t i = 0; i < strlen(rts_interval_string);i++){
                    if(!isdigit(rts_interval_string[i])){
                        proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                        exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                    }
                }

                if(sizeof(suseconds_t) == 8){
                    if(strcmp(rts_interval_string,"9223372036854775807") > 0){
                        proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                        exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                    }
                }else if(sizeof(suseconds_t) == 4){ 
                    if(strcmp(rts_interval_string,"2147483647") > 0){
                        proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                        exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                    }
                }else if(sizeof(suseconds_t) == 2){
                    if(strcmp(rts_interval_string,"65535") > 0){
                        proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                        exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                    }          
                }else if(sizeof(suseconds_t) == 1){
                    if(strcmp(rts_interval_string,"127") > 0){
                        proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                        exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                    }                          
                }
    
                suseconds_t rts_interval = (suseconds_t)strtol(rts_interval_string,NULL,10);
                if(rts_interval == 0 && errno != 0){
                    proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                    exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                }                    
                proximoth_rts_interval = rts_interval;

                proximoth_config_flags.r = true;
            }
            break;


            case 'h':{
                proximoth_config_print_usage();
                proximoth_error = PROXIMOTH_SUCCESS;
                exit((int)PROXIMOTH_SUCCESS);
            }
            break;

        
            case 'i':{ // Obligatory option
                
                bool interface_exists = false;
                struct ifaddrs* addrs = NULL;
                getifaddrs(&addrs);
                struct ifaddrs* iterator = NULL;

                if(strnlen(optarg,IFNAMSIZ) == IFNAMSIZ){
                    proximoth_error = PROXIMOTH_ERROR_ARGUMENT_INVALID;
                    exit((int)PROXIMOTH_ERROR_ARGUMENT_INVALID);
                }

                for(iterator = addrs; iterator != NULL; iterator = iterator->ifa_next){

                    if(strncmp(iterator->ifa_name,optarg,IFNAMSIZ-1) == 0){
                        interface_exists = true;
                        break;
                    }
                }

                freeifaddrs(addrs);

                if(interface_exists){
                    strncpy(proximoth_interface_name,optarg,strlen(optarg));

                    if(proximoth_interface_get_state(proximoth_interface_name,&proximoth_interface_state) == -1){
                        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_STATE;
                        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_STATE);
                    }

                    if(proximoth_interface_state != PROXIMOTH_INTERFACE_STATE_UP){
                        proximoth_error = PROXIMOTH_ERROR_INTERFACE_NOT_UP;
                        exit((int)PROXIMOTH_ERROR_INTERFACE_NOT_UP);
                    }

                    if(proximoth_interface_get_mode(proximoth_interface_name,&proximoth_interface_mode) == -1){
                        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_MODE;
                        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_MODE);
                    }

                    if(proximoth_interface_mode != PROXIMOTH_INTERFACE_MODE_MONITOR){
                        proximoth_error = PROXIMOTH_ERROR_INTERFACE_NO_MONITOR;
                        exit((int)PROXIMOTH_ERROR_INTERFACE_NO_MONITOR);
                    }


                    if((proximoth_interface_handle = pcap_open_live(optarg,5000,1,150,proximoth_error_buffer)) == NULL){
                        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_OPEN;
                        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_OPEN);
                    }

                    if(proximoth_interface_get_channel(proximoth_interface_name,&proximoth_interface_channel) == -1){
                        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_CHANNEL;
                        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_CHANNEL);
                    }

                    proximoth_config_flags.i = true;

                }else{
                    proximoth_error = PROXIMOTH_ERROR_INTERFACE_NOT_FOUND;
                    exit((int)PROXIMOTH_ERROR_INTERFACE_NOT_FOUND);
                }

            }
            break;


            case 'o':{

                if((proximoth_config_file_out = fopen(optarg,"w+")) == NULL ){
                    proximoth_error = PROXIMOTH_ERROR_FILE_CANNOT_CREATE;
                    exit((int)PROXIMOTH_ERROR_FILE_CANNOT_CREATE);
                }

                proximoth_config_flags.o = true;

            }
            break;

            case 'b':{
                if(!proximoth_mac_validate_mac_string(optarg)){
                    proximoth_error = PROXIMOTH_ERROR_BSSID_MAC_INVALID;
                    exit((int)PROXIMOTH_ERROR_BSSID_MAC_INVALID);
                }

                // MAC Address for BSSID is valid
                byte_t temp_bssid_mac[6] = {0};
                sscanf(optarg,"%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",&temp_bssid_mac[0],\
                                                            &temp_bssid_mac[1],\
                                                            &temp_bssid_mac[2],\
                                                            &temp_bssid_mac[3],\
                                                            &temp_bssid_mac[4],\
                                                            &temp_bssid_mac[5]);

                memcpy(proximoth_config_bssid_mac,temp_bssid_mac,6);
                proximoth_config_flags.b = true;

            }
            break;


            case ':':{
                proximoth_error = PROXIMOTH_ERROR_OPTION_NEEDS_ARG;
                exit((int)PROXIMOTH_ERROR_OPTION_NEEDS_ARG);
            }
            break;


            case '?':{
                proximoth_error = PROXIMOTH_ERROR_OPTION_INVALID;
                exit((int)PROXIMOTH_ERROR_OPTION_INVALID);
            }
            break;

        }
    }
    
    if( !(argc-optind == 1) ){
        proximoth_error = PROXIMOTH_ERROR_TARGET_MAC_INVALID;
        exit((int)PROXIMOTH_ERROR_TARGET_MAC_INVALID);
    }

    byte_t temp_target_mac[6] = {0};
    if(!proximoth_mac_validate_mac_string(argv[optind])){
        proximoth_error = PROXIMOTH_ERROR_TARGET_MAC_INVALID;
        exit((int)PROXIMOTH_ERROR_TARGET_MAC_INVALID);
    }

    sscanf(argv[optind],"%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX", &temp_target_mac[0],\
                                                        &temp_target_mac[1],\
                                                        &temp_target_mac[2],\
                                                        &temp_target_mac[3],\
                                                        &temp_target_mac[4],\
                                                        &temp_target_mac[5]);

    memcpy(proximoth_config_target_mac,temp_target_mac,6);

    if(!proximoth_config_flags.b){
        byte_t temp_mac[6];
        proximoth_mac_generate_random_mac(temp_mac);
        memcpy(proximoth_config_bssid_mac,temp_mac,6);
    }

    if(proximoth_config_flags.a == false){
        proximoth_config_bssid_mac[0] &= 0xFC; // 0b11111100
    }


    if(!proximoth_config_flags.i){
        proximoth_error = PROXIMOTH_ERROR_INTERFACE_NOT_SPECIFIED;
        exit((int)PROXIMOTH_ERROR_INTERFACE_NOT_SPECIFIED);
    }

    proximoth_mac_convert_mac_to_string(proximoth_config_bssid_mac,proximoth_config_bssid_mac_string);
    proximoth_mac_convert_mac_to_string(proximoth_config_target_mac,proximoth_config_target_mac_string);

    // Setting the filter for packet capturing.
    char proximoth_config_capture_filter[128] = "not wlan addr2 ";
    strncat(proximoth_config_capture_filter,proximoth_config_bssid_mac_string,PROXIMOTH_MAC_STRING_SIZE-1);
    struct bpf_program filter = {0};
    if(pcap_compile(proximoth_interface_handle,&filter,proximoth_config_capture_filter,0,PCAP_NETMASK_UNKNOWN)){
        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_COMPILE_FILTER;
        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_COMPILE_FILTER);
    }
    if(pcap_setfilter(proximoth_interface_handle,&filter)){
        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_SET_FILTER;
        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_SET_FILTER);
    }

    if(proximoth_config_flags.o){
        time_t t = proximoth_config_start_time.tv_sec;
        struct tm broken_start_time = *localtime(&t);
        char formatted_start_time[32] = {0};
        strftime(formatted_start_time,21,"%Y/%m/%d %X",&broken_start_time);

        fprintf(proximoth_config_file_out, 
            "Proximoth (Version %s)\n"
            "BSSID MAC       : %s\n"
            "Target MAC      : %s\n"
            "Interface       : %s (Channel: %d)\n"
            "Start Time      : %s\n"
            "GMT Offset      : %ld seconds\n\n"
            "Listening for CTS...\n",
            PROXIMOTH_VERSION,
            proximoth_config_bssid_mac_string,
            proximoth_config_target_mac_string,
            proximoth_interface_name,
            proximoth_interface_channel,
            formatted_start_time,
            broken_start_time.tm_gmtoff);

        fflush(proximoth_config_file_out);
    }

    if(proximoth_config_flags.t){
        time_t t = proximoth_config_start_time.tv_sec;
        struct tm broken_start_time = *localtime(&t);
        char formatted_start_time[32] = {0};
        strftime(formatted_start_time,21,"%Y/%m/%d %X",&broken_start_time);

        fprintf(stdout, 
            "Proximoth (Version %s)\n"
            "BSSID MAC       : %s\n"
            "Target MAC      : %s\n"
            "Interface       : %s (Channel: %d)\n"
            "Start Time      : %s\n"
            "GMT Offset      : %ld seconds\n\n"
            "Listening for CTS...\n",
            PROXIMOTH_VERSION,
            proximoth_config_bssid_mac_string,
            proximoth_config_target_mac_string,
            proximoth_interface_name,
            proximoth_interface_channel,
            formatted_start_time,
            broken_start_time.tm_gmtoff);

        fflush(stdout);
    }else{
        proximoth_display_set_cursor_visibility(false);

        struct sigaction proximoth_signal_action_window_resize = {0};
        proximoth_signal_action_window_resize.sa_handler = proximoth_signal_display_resize_event;
        proximoth_signal_action_window_resize.sa_flags = SA_RESTART;
        
        if(sigaction(SIGWINCH,&proximoth_signal_action_window_resize,NULL) == -1){
            proximoth_error = PROXIMOTH_ERROR_SIGNAL_BAD_INIT;
            exit((int)PROXIMOTH_ERROR_SIGNAL_BAD_INIT);
        }

    }

    if(proximoth_config_flags.d){

        proximoth_config_dump_handle = pcap_dump_open(proximoth_interface_handle,proximoth_config_dump_file);
        if(proximoth_config_dump_handle == NULL){
            proximoth_error = PROXIMOTH_ERROR_PCAP_FILE_CANNOT_CREATE;
            exit((int)PROXIMOTH_ERROR_FILE_CANNOT_CREATE);
        }
        
    }

    if(pcap_setnonblock(proximoth_interface_handle,true,proximoth_error_buffer)){
        proximoth_error = PROXIMOTH_ERROR_INTERFACE_CANNOT_SET_NONBLOCK;
        exit((int)PROXIMOTH_ERROR_INTERFACE_CANNOT_SET_NONBLOCK);
    }

    proximoth_config_finished = true;
    proximoth_error = PROXIMOTH_SUCCESS;
    return;
}

void proximoth_config_reset(void){
    struct timeval current;
    gettimeofday(&current,0);

    if(proximoth_config_flags.o && proximoth_config_finished){

        suseconds_t t = (current.tv_sec - proximoth_config_start_time.tv_sec)*1000000 + ( current.tv_usec - proximoth_config_start_time.tv_usec);
        time_t t_diff = t / 1000000;
        suseconds_t t_u_diff = t % 1000000;
        
        fprintf(proximoth_config_file_out, 
            "\nTotal Frames Catched  : %lu\n"
            "Total RTS Injected    : %lu\n"
            "Total CTS Catched     : %lu\n"
            "Time Elapsed          : %ld.%ld seconds\n",
            proximoth_total_frames_catched,
            proximoth_rts_total_injected,
            proximoth_cts_total_catched,
            t_diff, t_u_diff);

        time_t epoch = proximoth_cts_last_time.tv_sec;
        if(epoch == 0){
            fprintf(proximoth_config_file_out,"Last CTS Time         : --:--:--\n");
        }else{
            struct tm* bt = localtime(&epoch);
            if(bt == NULL){
                exit((int)PROXIMOTH_ERROR_LOCALTIME);
            }
            fprintf(proximoth_config_file_out,"Last CTS Time         : %02d:%02d:%02d\n",
            bt->tm_hour, bt->tm_min, bt->tm_sec);
        }


            

        fflush(proximoth_config_file_out);
        fclose(proximoth_config_file_out);
        fflush(stdout);
    }



    if(proximoth_config_flags.t && proximoth_config_finished){

        suseconds_t t = (current.tv_sec - proximoth_config_start_time.tv_sec)*1000000 + ( current.tv_usec - proximoth_config_start_time.tv_usec);
        time_t t_diff = t / 1000000;
        suseconds_t t_u_diff = t % 1000000;

        fprintf(stdout, 
            "\nTotal Frames Catched  : %lu\n"
            "Total RTS Injected    : %lu\n"
            "Total CTS Catched     : %lu\n"
            "Time Elapsed          : %ld.%ld seconds\n",
            proximoth_total_frames_catched,
            proximoth_rts_total_injected,
            proximoth_cts_total_catched,
            t_diff, t_u_diff);

        time_t epoch = proximoth_cts_last_time.tv_sec;
        if(epoch == 0){
            fprintf(stdout,"Last CTS Time         : --:--:--\n");
        }else{
            struct tm* bt = localtime(&epoch);
            if(bt == NULL){
                exit((int)PROXIMOTH_ERROR_LOCALTIME);
            }
            fprintf(stdout,"Last CTS Time         : %02d:%02d:%02d\n",
            bt->tm_hour, bt->tm_min, bt->tm_sec);
        }

        fflush(stdout);
    }

    if(proximoth_config_flags.d && proximoth_config_finished){
        pcap_dump_close(proximoth_config_dump_handle);
    }
    if(proximoth_interface_handle != NULL)
        pcap_close(proximoth_interface_handle);

    if(proximoth_config_flags.t == false)
        proximoth_display_set_cursor_visibility(true);

    if(proximoth_error != PROXIMOTH_SUCCESS){
        system("clear");
        proximoth_display_set_cursor_position(1,1);
        fprintf(stderr,"Proximoth Error: %s\n",proximoth_error_get_message(proximoth_error));
        fflush(stderr);
    }

}
