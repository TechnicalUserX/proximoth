#include "../include/proximoth_frame_sniffer.hpp"
#include "../include/proximoth_radiotap.hpp"
#include "../include/proximoth_config.hpp"
#include "../include/proximoth_misc_tools.hpp"



pthread_t proximoth_frame_sniffer_thread;

bool proximoth_frame_sniffer_thread_shutdown_request = false;

// CTS Information
double proximoth_cts_catch_period = 0;

uint64_t proximoth_cts_total_catched = 0;

struct timeval proximoth_cts_last_time = {0};

// Total Frames
uint64_t proximoth_total_frames_catched = 0;

int8_t proximoth_cts_last_signal_strength = -100;

void* proximoth_frame_sniffer(void* arg){
    // "arg" is not used

    pcap_t* handle = (pcap_t*)proximoth_config_interface_handle;
    
    struct pcap_pkthdr* hdr;
    const byte_t* data;


    while(1){

        if(proximoth_frame_sniffer_thread_shutdown_request){
            pthread_exit(NULL);
        }

        int status = pcap_next_ex(handle,&hdr,&data);

        if(status != 1){
            continue;
        }        

        if(data == NULL){
            continue;
        }

        if(hdr->caplen <  sizeof(struct ieee80211_radiotap_header)){
            continue;;
        }


        proximoth_total_frames_catched++;

        struct ieee80211_radiotap_header* radiotap = (struct ieee80211_radiotap_header*)data;



        const byte_t* frame = data + radiotap->length;

        if(frame[0] == IEEE80211_FRAME_CONTROL_CTS){


            struct ieee80211_clear_to_send* cts = (struct ieee80211_clear_to_send*)frame;

            if(proximoth_toolset_confirm_generated_mac(cts->receiver_address)){

                // CTS Catch!
                proximoth_cts_total_catched++;

                time_t epoch = time(NULL); //Getting epoch...
                struct tm local = {0};
                localtime_r(&epoch, &local);
                epoch += local.tm_gmtoff; // Finalising epoch to fit current GMT.
                struct timeval current_time;
                gettimeofday(&current_time,0);
                current_time.tv_sec = epoch; // Finalising current_time with GMT correct epoch

                byte_t* antsignal = ieee80211_radiotap_header_find_field(IEEE80211_RADIOTAP_DBM_ANTSIGNAL,(byte_t*)radiotap);

                
                if(antsignal != NULL){
                    proximoth_cts_last_signal_strength = (*((char*)antsignal));
                }


                if(proximoth_cts_last_time.tv_sec == 0 and proximoth_cts_last_time.tv_usec == 0){
                    double current_period = (current_time.tv_sec - proximoth_config_start_time.tv_sec) + (current_time.tv_usec*1e-6 - proximoth_config_start_time.tv_usec*1e-6);

                    if(proximoth_config_flags.f){
                        int t1 = 0;

                        if(proximoth_cts_last_time.tv_sec == 0 && proximoth_cts_last_time.tv_usec == 0){
                            t1 = proximoth_config_start_time.tv_sec % (3600*24);
                        }else{
                            t1 = proximoth_cts_last_time.tv_sec % (3600*24);
                        }

                        int t2 = current_time.tv_sec % (3600*24);

                        int h1 = 0,m1 = 0,s1 = 0;
                        int h2 = 0,m2 = 0,s2 = 0;
                        
                        s1 = ( t1 -= (m1 =  (t1 -= (h1 = t1/3600)*3600)/60)*60);
                        s2 = ( t2 -= (m2 =  (t2 -= (h2 = t2/3600)*3600)/60)*60);

                        fprintf(proximoth_config_file_out," -> [%02d:%02d:%02d] <-> [%02d:%02d:%02d] : Period %.6lf seconds : dBm %hhd\n",\
                        h1,m1,s1,h2,m2,s2,current_period,proximoth_cts_last_signal_strength);
                        fflush(proximoth_config_file_out);

                    }

                    proximoth_cts_catch_period = current_period;


                }else{

                    double current_period = (current_time.tv_sec - proximoth_cts_last_time.tv_sec) + (current_time.tv_usec*1e-6 - proximoth_cts_last_time.tv_usec*1e-6);


                    if(proximoth_config_flags.f){

                        int t1 = 0;

                        if(proximoth_cts_last_time.tv_sec == 0 && proximoth_cts_last_time.tv_usec == 0){
                            t1 = proximoth_config_start_time.tv_sec % (3600*24);
                        }else{
                            t1 = proximoth_cts_last_time.tv_sec % (3600*24);
                        }


                        int t2 = current_time.tv_sec % ( 3600*24);
                        int h1 = 0,m1 = 0,s1 = 0;
                        int h2 = 0,m2 = 0,s2 = 0;
                        
                        s1 = ( t1 -= (m1 =  (t1 -= (h1 = t1/3600)*3600)/60)*60);
                        s2 = ( t2 -= (m2 =  (t2 -= (h2 = t2/3600)*3600)/60)*60);

                        fprintf(proximoth_config_file_out," -> [%02d:%02d:%02d] <-> [%02d:%02d:%02d] : Period %.6lf seconds : dBm %hhd\n",\
                        h1,m1,s1,h2,m2,s2,current_period,proximoth_cts_last_signal_strength);

                        fflush(proximoth_config_file_out);
                    }

                    if(current_period > proximoth_cts_catch_period){
                        proximoth_cts_catch_period = current_period;
                    }

                }

                proximoth_cts_last_time = current_time;
                
                if(proximoth_config_flags.d){
                    pcap_dump((u_char*)proximoth_config_dump_handle,hdr,data);
                }

            }

        }

    }

}