#include "../include/proximoth.h"
#include "../include/proximoth_config.h"
#include "../include/proximoth_error.h"
#include "../include/proximoth_threads.h"
#include "../include/proximoth_frame_sniffer.h"

int main(int argc, char* argv[]){


    int ret_code = 0;
    
    if( (ret_code = proximoth_config(argc,argv)) != PROXIMOTH_SUCCESS){
        const char* msg =  proximoth_error_get_message(ret_code);
        fprintf(stdout,"%s\n",msg);
        fflush(stdout);
        exit(ret_code);
    }

    if( (ret_code = proximoth_threads_start()) != PROXIMOTH_SUCCESS){
        const char* msg = proximoth_error_get_message(ret_code);
        fprintf(stdout,"%s\n",msg);
        fflush(stdout);        
        exit(ret_code);        
    }

    if( (ret_code = proximoth_threads_wait()) != PROXIMOTH_SUCCESS){
        const char* msg = proximoth_error_get_message(ret_code);
        fprintf(stdout,"%s\n",msg);
        fflush(stdout);        
        exit(ret_code);        
    }

    if(proximoth_config_flags.d){
        pcap_dump_close(proximoth_config_dump_handle);
    }

    pcap_close(proximoth_config_interface_handle);

    exit(PROXIMOTH_SUCCESS);
}