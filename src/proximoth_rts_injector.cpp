#include "../include/proximoth_rts_injector.hpp"
#include "../include/proximoth_radiotap.hpp"
#include "../include/proximoth_config.hpp"
#include "../include/proximoth_misc_tools.hpp"
#include "../include/proximoth_error.hpp"
#include "../include/proximoth_frame_sniffer.hpp"

pthread_t proximoth_rts_injector_thread;

bool proximoth_rts_injector_thread_shutdown_request = false;

uint64_t proximoth_rts_total_injected = 0;

uint32_t proximoth_rts_interval = 500000;

void* proximoth_rts_injector(void* arg){
    // arg is not used

    pcap_t* handle = (pcap_t*)proximoth_config_interface_handle;
    
    byte_t frame[sizeof(ieee80211_radiotap_template) + sizeof(ieee80211_request_to_send)];

    ieee80211_request_to_send rts;
    rts.version = 0b00; 
    rts.type = 0b01; 
    rts.subtype = 0b1011;

    rts.duration = 0;
    rts.flags = 0;
    memcpy(rts.receiver_address,proximoth_config_target_mac,6);
    memcpy(rts.transmitter_address,proximoth_config_bssid_mac,6);
    
    memcpy(frame,ieee80211_radiotap_template,8);
    memcpy(frame+8,&rts,sizeof(rts));

    while(1){

        if(proximoth_rts_injector_thread_shutdown_request){
            pthread_exit(NULL);
        }
        
        pcap_inject(handle,frame,sizeof(frame));
        proximoth_rts_total_injected++;
        usleep(proximoth_rts_interval);
    }
    
    return NULL;
}

void proximoth_rts_injector_wait(void){

        int fd[2] = {0};
        pipe(fd);
        int flags = 0;
        fcntl(fd[0],F_GETFL,&flags);
        flags |= O_NONBLOCK;
        fcntl(fd[0],F_SETFL,flags);

        pid_t pid = fork();

        // Catching...
        if(pid == 0){
            fflush(stdout);
            // Child, responsible for catching...
            pcap_t* handle = (pcap_t*)proximoth_config_interface_handle;
            struct pcap_pkthdr hdr;
            struct ieee80211_radiotap_header* radiotap_header;

            time_t rts_seen = 0;

            while(1){
                
                const byte_t* data = pcap_next(handle,&hdr);
                radiotap_header = (struct ieee80211_radiotap_header*)data;
                const byte_t* frame = data + radiotap_header->length;
                if(frame[0] == IEEE80211_FRAME_CONTROL_RTS){
                    struct ieee80211_request_to_send* rts;
                    rts = (struct ieee80211_request_to_send*)frame;
                    if(proximoth_toolset_confirm_generated_mac(rts->transmitter_address)){
                        rts_seen = time(NULL);
                        write(fd[1],&rts_seen,sizeof(time_t));
                    }
                }

            }

        }else{
            // Parent, responsible for retrieving data from child to interpret 
            printf("Waiting for all RTS frames to disappear: ");
            fflush(stdout);

            time_t rts_seen = time(NULL);

            do{

                int slide = printf("%ld",time(NULL)-rts_seen);
                fflush(stdout);

                time_t temp = 0;
                int r = read(fd[0],&temp,sizeof(time_t));

                if(r != -1){
                    rts_seen = temp;
                }

                printf("\033[%dD",slide);
            }while(time(NULL) - rts_seen <= 10);

            if(kill(pid,SIGKILL) == -1){
                exit(PROXIMOTH_ERROR_PROCESS_CHILD_BAD_KILL);
            }

        }

    close(fd[0]);
    close(fd[1]);
}