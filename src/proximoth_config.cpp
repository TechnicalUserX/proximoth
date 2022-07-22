#include "../include/proximoth_config.hpp"
#include "../include/proximoth_error.hpp"
#include "../include/proximoth_misc_tools.hpp"
#include "../include/proximoth_display_manager.hpp"
#include "../include/proximoth_frame_sniffer.hpp"
#include "../include/proximoth_rts_injector.hpp"
#include "../include/proximoth_signal.hpp"
#include "../include/proximoth_interface_mgmt.hpp"
#include "../include/proximoth_radiotap.hpp"

char proximoth_config_dump_file[PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN+1] = {0};

pcap_dumper_t* proximoth_config_dump_handle = NULL;

struct timeval proximoth_config_start_time;

byte_t proximoth_config_bssid_mac[6] = {0};

byte_t proximoth_config_target_mac[6] = {0};

FILE* proximoth_config_file_out = NULL;

struct proximoth_config_flags_t proximoth_config_flags;


pcap_t* proximoth_config_interface_handle = NULL;

char proximoth_config_error_buffer[256] = {0};

time_t proximoth_config_time_gmt_offset = 0;

bool proximoth_config_finished = 0;

void proximoth_config_print_usage(void){

    printf( "USAGE:\n\n"

            "proximoth [flags] <TARGET>\n\n"

            " <TARGET>                            : MAC address of the target, no need a flag.\n\n"

            "FLAGS:\n\n"

            " -h, --help                          : Print this screen.\n\n"

            " -f <FILE>, --file <FILE>            : File name <FILE> to write measurements after the close of Proximoth.\n\n"
            
            " -b <BSSID>, --bssid <BSSID>         : Custom BSSID to be injected as sender MAC address.\n"
            "                                       Only first 3 pairs are considered. Last 3 pairs are ignored.\n\n"
            
            " -i <IFACE>, --interface <IFACE>     : Wireless interface to use packet injection and sniffing.\n\n"

            " -w, --wait-rts                      : Wait until the surrounding is free from all injected RTS frames for 10 seconds.\n\n"

            " -d <FILE>, --dump-file <FILE>       : Write all CTS frames to a pcap file.\n\n"

            " -r <NUM>, --rts-interval <NUM>      : Number of microseconds between every RTS injection, default is 500000.\n\n"
            
            " --version                           : Prints version number and author information.\n\n");
            fflush(stdout);
}

void proximoth_config_print_version(void){

    printf( "Proximoth - Written by TechnicalUserX\n"
            "Version %s\n",PROXIMOTH_VERSION);

}

static const struct option long_options[] = {
    { "help"        ,no_argument,       NULL,    'h'},    
    { "interface"   ,required_argument, NULL,    'i'},
    { "file"        ,required_argument, NULL,    'f'},
    { "bssid"       ,required_argument, NULL,    'b'},
    { "wait-rts"    ,required_argument, NULL,    'w'},
    { "rts-interval",required_argument, NULL,    'r'},
    { "dump-file"   ,required_argument, NULL,    'd'},
    { "version"     ,no_argument,       NULL, 1000},
    { NULL,         0, NULL,   0   }   /* Required at end of array.  */
};

int proximoth_config(int argc, char* argv[]){

    // Synchronising the streams of C++ and C...
    std::ios_base::sync_with_stdio(true);

    // Seeding the PRNG...
    srandom(time(NULL));


    gettimeofday(&proximoth_config_start_time,0);
    time_t epoch = time(NULL); //Getting epoch...
    struct tm local = {0};
    localtime_r(&epoch, &local);
    proximoth_config_time_gmt_offset = local.tm_gmtoff;
    proximoth_config_start_time.tv_sec += proximoth_config_time_gmt_offset;


    ieee80211_radiotap_initialize_align_size(rtap_namespace_sizes);

    struct sigaction proximoth_signal_action_window_resize;
    proximoth_signal_action_window_resize.sa_handler = proximoth_signal_display_manager_resize_event;
    proximoth_signal_action_window_resize.sa_flags = SA_RESTART;
    
    if(sigaction(SIGWINCH,&proximoth_signal_action_window_resize,NULL) == -1){
        return PROXIMOTH_ERROR_SIGNAL_BAD_INIT;
    }

    struct sigaction proximoth_signal_action_interrupt;
    proximoth_signal_action_interrupt.sa_handler = proximoth_signal_program_interrupt;
    
    if(sigaction(SIGINT,&proximoth_signal_action_interrupt,NULL) == -1){
        return PROXIMOTH_ERROR_SIGNAL_BAD_INIT;
    }


    optind = 0;
    int opt = 0;

    while((opt = getopt_long(argc, argv, ":wi:f:b:r:d:h",long_options,NULL)) != -1){

        switch(opt){

            case 1000:{
                proximoth_config_print_version();
                exit(PROXIMOTH_SUCCESS);
            }
            break;

            case 'd':{
                strncpy(proximoth_config_dump_file,optarg,PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN);
                proximoth_config_flags.d = true;
            }
            break;


            case 'r':{
                
                char rts_interval_string[128] = {0};
                strcpy(rts_interval_string,optarg);
                int rts_interval = strtoll(rts_interval_string,NULL,10);

                if(rts_interval >= 100000)
                    rts_interval = 100000;
                else if(rts_interval<0)
                    rts_interval=0;
                
                proximoth_rts_interval = rts_interval;

                proximoth_config_flags.r = true;
            }
            break;


            case 'h':{
                proximoth_config_print_usage();
                exit(PROXIMOTH_SUCCESS);
            }
            break;


            case 'w':{

                proximoth_config_flags.w = true;

            }
            break;

        
            case 'i':{ // Obligatory argument
                
                bool interface_exists = false;
                struct ifaddrs* addrs;
                getifaddrs(&addrs);
                struct ifaddrs* iterator;

                for(iterator = addrs; iterator != NULL; iterator = iterator->ifa_next){

                    if(strcmp(iterator->ifa_name,optarg) == 0){
                        // Interface exists!
                        interface_exists = true;
                        break;
                    }
                }
                freeifaddrs(addrs);



                if(interface_exists){
                    strncpy(proximoth_interface_name,optarg,strlen(optarg));

                    if(proximoth_interface_get_state(proximoth_interface_name,&proximoth_interface_state) == -1){
                        return PROXIMOTH_ERROR_GENERIC;
                    }

                    if(proximoth_interface_state != PROXIMOTH_INTERFACE_STATE_UP){
                        return PROXIMOTH_ERROR_INTERFACE_NOT_UP;
                    }

                    if(proximoth_interface_get_mode(proximoth_interface_name,&proximoth_interface_mode) == -1){
                        return PROXIMOTH_ERROR_GENERIC;
                    }

                    if(proximoth_interface_mode != PROXIMOTH_INTERFACE_MODE_MONITOR){
                        return PROXIMOTH_ERROR_INTERFACE_NO_MONITOR;
                    }


                    if((proximoth_config_interface_handle = pcap_open_live(optarg,3000,1,150,proximoth_config_error_buffer)) == NULL){
                        return PROXIMOTH_ERROR_INTERFACE_CANNOT_OPEN;
                    }

                    if(proximoth_interface_get_channel(proximoth_interface_name,&proximoth_interface_channel) == -1){
                        return PROXIMOTH_ERROR_GENERIC;
                    }

                    proximoth_config_flags.i = true;

                }else{
                    return PROXIMOTH_ERROR_INTERFACE_NOT_FOUND;
                }


            }
            break;


            case 'f':{

                if( (proximoth_config_file_out = fopen(optarg,"w+")) == NULL ){
                    return PROXIMOTH_ERROR_FILE_CANNOT_CREATE;
                }

                proximoth_config_flags.f = true;

            }
            break;


            case 'b':{
                if(!proximoth_toolset_validate_mac_address(optarg)){
                    return PROXIMOTH_ERROR_BSSID_MAC_INVALID;
                }

                // MAC Address for BSSID is valid
                byte_t temp_bssid_mac[6] = {0};
                sscanf(optarg,"%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",&temp_bssid_mac[0],\
                                                            &temp_bssid_mac[1],\
                                                            &temp_bssid_mac[2],\
                                                            &temp_bssid_mac[3],\
                                                            &temp_bssid_mac[4],\
                                                            &temp_bssid_mac[5]);

                temp_bssid_mac[3] = proximoth_toolset_powm(temp_bssid_mac[0],30,256);
                temp_bssid_mac[4] = proximoth_toolset_powm(temp_bssid_mac[1],40,256);
                temp_bssid_mac[5] = proximoth_toolset_powm(temp_bssid_mac[2],50,256);

                memcpy(proximoth_config_bssid_mac,temp_bssid_mac,6);
                proximoth_config_flags.b = true;


            }
            break;


            case ':':{
                return PROXIMOTH_ERROR_OPTION_NEEDS_ARG;
            }
            break;


            case '?':{
                return PROXIMOTH_ERROR_OPTION_INVALID;
            }
            break;

        }
    }
    
    

    if( !(argc-optind == 1) ){

        return(PROXIMOTH_ERROR_TARGET_MAC_INVALID);
    }



    
    char temp_target_mac[6] = {0};
    if(!proximoth_toolset_validate_mac_address(argv[optind])){
        return PROXIMOTH_ERROR_TARGET_MAC_INVALID;
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
        proximoth_toolset_generate_random_mac(temp_mac);
        memcpy(proximoth_config_bssid_mac,temp_mac,6);
    }


    if(!proximoth_config_flags.i){
        return PROXIMOTH_ERROR_INTERFACE_NOT_SPECIFIED;
    }

    if(proximoth_config_flags.w){
        proximoth_rts_injector_wait();
        
    }

    if(proximoth_config_flags.f){

        fprintf(proximoth_config_file_out,  "[=============================================================]\n"
                                            "                    Proximoth (Version %s)\n"
                                            "[=============================================================]\n"
                                            " - BSSID MAC   : %s\n"
                                            " - Target MAC  : %s\n"
                                            " - Interface   : %s (Channel: %d)\n"
                                            "[=============================================================]\n",\
                                            PROXIMOTH_VERSION,
                                            proximoth_toolset_convert_mac_to_string(proximoth_config_bssid_mac).c_str(),\
                                            proximoth_toolset_convert_mac_to_string(proximoth_config_target_mac).c_str(),\
                                            proximoth_interface_name,\
                                            proximoth_interface_channel);
        fflush(proximoth_config_file_out);



    }

    if(proximoth_config_flags.d){

        proximoth_config_dump_handle = pcap_dump_open(proximoth_config_interface_handle,proximoth_config_dump_file);
        if(proximoth_config_dump_handle == NULL){
            return PROXIMOTH_ERROR_PCAP_FILE_CANNOT_CREATE;
        }
        

    }

    proximoth_config_finished = true;

    return PROXIMOTH_SUCCESS;
}