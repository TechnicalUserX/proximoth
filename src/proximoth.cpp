#include "../include/proximoth.hpp"
#include "../include/proximoth_config.hpp"
#include "../include/proximoth_error.hpp"
#include "../include/proximoth_threads.hpp"
#include "../include/proximoth_frame_sniffer.hpp"

int main(int argc, char* argv[]){


    int ret_code = 0;
    
    if( (ret_code = proximoth_config(argc,argv))    != PROXIMOTH_SUCCESS){
        std::cout << proximoth_error_get_message(ret_code) << std::endl;
        exit(ret_code);
    }

    if( (ret_code = proximoth_threads_start())      != PROXIMOTH_SUCCESS){
        std::cout << proximoth_error_get_message(ret_code) << std::endl;
        exit(ret_code);        
    }

    if( (ret_code = proximoth_threads_wait())       != PROXIMOTH_SUCCESS){
        std::cout << proximoth_error_get_message(ret_code) << std::endl;
        exit(ret_code);        
    }

    if(proximoth_config_flags.d){
        pcap_dump_close(proximoth_config_dump_handle);
    }

    pcap_close(proximoth_config_interface_handle);

    exit(PROXIMOTH_SUCCESS);
}