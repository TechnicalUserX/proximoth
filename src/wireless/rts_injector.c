#include <proximoth/utility/config.h>
#include <proximoth/utility/error.h>
#include <proximoth/wireless/mac.h>
#include <proximoth/wireless/cts_sniffer.h>
#include <proximoth/wireless/rts_injector.h>
#include <proximoth/wireless/radiotap.h>

pthread_t proximoth_rts_injector_thread;

atomic_bool proximoth_rts_injector_thread_shutdown_request = false;

uint64_t proximoth_rts_total_injected = 0;

suseconds_t proximoth_rts_interval = 500000;

void* proximoth_rts_injector(void* arg){
    // arg is not used

    pcap_t* handle = (pcap_t*)proximoth_config_interface_handle;
    
    byte_t frame[sizeof(proximoth_radiotap_template) + sizeof(struct proximoth_request_to_send)];

    struct proximoth_request_to_send rts;
    rts.version = 0x0; 
    rts.type = 0x1; 
    rts.subtype = 0xB; //0b1011;

    rts.duration = 0;
    rts.flags = 0;
    memcpy(rts.receiver_address,proximoth_config_target_mac,6);
    memcpy(rts.transmitter_address,proximoth_config_bssid_mac,6);
    
    memcpy(frame,proximoth_radiotap_template,8);
    memcpy(frame+8,&rts,sizeof(rts));

    while(1){
        if(proximoth_rts_injector_thread_shutdown_request){
            pthread_exit(NULL);
        }
        int inject_status = pcap_inject(handle,frame,sizeof(frame));

        if(inject_status < 0){
            if(errno == EAGAIN)
                continue;
            proximoth_error = PROXIMOTH_ERROR_INJECT;
            exit((int)PROXIMOTH_ERROR_INJECT);
        }else if(inject_status == sizeof(frame)){
            proximoth_rts_total_injected++;
        }

        struct timeval tv_start = {0};
        struct timeval tv_current = {0};
        suseconds_t dif = 0;
        gettimeofday(&tv_start,NULL);

        do{
            gettimeofday(&tv_current,NULL);
            dif = (tv_current.tv_sec - tv_start.tv_sec)*1000000 + (tv_current.tv_usec - tv_start.tv_usec);
        }while((dif < proximoth_rts_interval) && (proximoth_rts_injector_thread_shutdown_request == false));

    }
    
}

