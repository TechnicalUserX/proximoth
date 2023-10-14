#include <proximoth/utility/config.h>
#include <proximoth/wireless/cts_sniffer.h>
#include <proximoth/wireless/radiotap.h>
#include <proximoth/wireless/mac.h>
#include <proximoth/wireless/interface.h>

pthread_t proximoth_cts_sniffer_thread;

atomic_bool proximoth_cts_sniffer_thread_shutdown_request = false;

// CTS Information
double proximoth_cts_catch_period = 0;

uint64_t proximoth_cts_total_catched = 0;

struct timeval proximoth_cts_last_time = {0,0};

// Total Frames
uint64_t proximoth_total_frames_catched = 0;

int8_t proximoth_cts_last_signal_strength = -100;

void* proximoth_cts_sniffer(void* arg){
    // "arg" is not used

    pcap_t* handle = (pcap_t*)proximoth_interface_handle;
    
    struct pcap_pkthdr* hdr = NULL;
    const byte_t* data = NULL;

    while(1){

        if(proximoth_cts_sniffer_thread_shutdown_request){
            pthread_exit(NULL);
        }

        int status = pcap_next_ex(handle,&hdr,&data);

        if(status == 0){
            continue;
        }        

        if(status == -1){
            proximoth_error = PROXIMOTH_ERROR_SNIFF;
            exit((int)PROXIMOTH_ERROR_SNIFF);
        }

        if(data == NULL){
            continue;
        }
        
        if(hdr->caplen <  sizeof(struct proximoth_radiotap_header)){
            continue;;
        }

        proximoth_total_frames_catched++;

        struct proximoth_radiotap_header* radiotap = (struct proximoth_radiotap_header*)data;

        const byte_t* frame = data + radiotap->length;

        if(frame[0] == PROXIMOTH_CTS_FRAME){

            struct proximoth_clear_to_send* cts = (struct proximoth_clear_to_send*)frame;

            if(memcmp(cts->receiver_address,proximoth_config_bssid_mac,6) == 0){

                // CTS Catch!
                proximoth_cts_total_catched++;

                struct timeval current_time = hdr->ts;

                byte_t* antsignal = proximoth_radiotap_header_find_field(PROXIMOTH_RADIOTAP_DBM_ANTSIGNAL,(byte_t*)radiotap);
                
                if(antsignal != NULL){
                    proximoth_cts_last_signal_strength = (*((char*)antsignal));
                }

                double current_period = 0;

                if(proximoth_cts_last_time.tv_sec == 0 && proximoth_cts_last_time.tv_usec == 0){
                    current_period = (current_time.tv_sec - proximoth_config_start_time.tv_sec) + (current_time.tv_usec*1e-6 - proximoth_config_start_time.tv_usec*1e-6);
                    proximoth_cts_catch_period = current_period;
                }else{
                    current_period = (current_time.tv_sec - proximoth_cts_last_time.tv_sec) + (current_time.tv_usec*1e-6 - proximoth_cts_last_time.tv_usec*1e-6);
                    if(current_period > proximoth_cts_catch_period){
                        proximoth_cts_catch_period = current_period;
                    }
                }

                if(proximoth_config_flags.o){
                    static bool headers_printed = false;
                    if(headers_printed == false && proximoth_cts_total_catched > 0){
                        fprintf(proximoth_config_file_out, "\n%-16s%-16s%-21s%-16s\n",
                        "EPOCH", "DATE", "TIME", "SIGNAL");
                        headers_printed = true;

                    }
                    time_t t = current_time.tv_sec;
                    suseconds_t t_m = current_time.tv_usec;

                    struct tm* l;
                    if((l = localtime(&t)) == NULL){
                        proximoth_error = PROXIMOTH_ERROR_LOCALTIME;
                        exit((int)PROXIMOTH_ERROR_LOCALTIME);
                    }
                    struct tm broken_t = *l;

                    char formatted_t_date[32] = {0};
                    char formatted_t_time[32] = {0};

                    strftime(formatted_t_date,21,"%Y/%m/%d",&broken_t);
                    strftime(formatted_t_time,21,"%T",&broken_t);

                    fprintf(proximoth_config_file_out,"%-16" PRId64 "%-16s%-8s.%-12" PRId64 "dBm %-16" PRId8 "\n", 
                        (int64_t)t,
                        formatted_t_date,
                        formatted_t_time,
                        (int64_t)t_m,
                        proximoth_cts_last_signal_strength
                    );
                    fflush(proximoth_config_file_out);
                }

                if(proximoth_config_flags.t){
                    static bool headers_printed = false;
                    if(headers_printed == false && proximoth_cts_total_catched > 0){
                        fprintf(stdout, "\n%-16s%-16s%-21s%-16s\n",
                        "EPOCH", "DATE", "TIME", "SIGNAL");
                        headers_printed = true;
                    }

                    time_t t = current_time.tv_sec;
                    suseconds_t t_m = current_time.tv_usec;

                    struct tm* l;
                    if((l = localtime(&t)) == NULL){
                        proximoth_error = PROXIMOTH_ERROR_LOCALTIME;
                        exit((int)PROXIMOTH_ERROR_LOCALTIME);
                    }
                    struct tm broken_t = *l;

                    char formatted_t_date[32] = {0};
                    char formatted_t_time[32] = {0};

                    strftime(formatted_t_date,21,"%Y/%m/%d",&broken_t);
                    strftime(formatted_t_time,21,"%T",&broken_t);

                    fprintf(stdout,"%-16" PRId64 "%-16s%-8s.%-12" PRId64 "dBm %-16" PRId8 "\n", 
                        (int64_t)t,
                        formatted_t_date,
                        formatted_t_time,
                        (int64_t)t_m,
                        proximoth_cts_last_signal_strength
                    );
                    fflush(stdout);
                }

                proximoth_cts_last_time = current_time;
                
                if(proximoth_config_flags.d){
                    pcap_dump((u_char*)proximoth_config_dump_handle,hdr,data);
                }


            }

        }

    }

}