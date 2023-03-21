#include "../include/proximoth_toolset.h"
#include "../include/proximoth_error.h"


uint64_t proximoth_toolset_powm(uint64_t base, uint64_t pow, uint64_t modulo){

    uint64_t m = 1;
    for(uint64_t i = 0; i < pow; i++){

        m = ((m % modulo)  * (base % modulo)) % modulo;
    }
    return m;
}

bool proximoth_toolset_validate_mac_string(const char* mac_string){
    if((int)strlen(mac_string) != 17){
        return false;
    }

    char temp_mac_string[18] = {0};
    strncpy(temp_mac_string,mac_string,17);


    for(int i = 0; i < 17; i++)
        temp_mac_string[i] = tolower(temp_mac_string[i]);
    

    for(int i = 1; i <= 17; i++){
        int c = temp_mac_string[i-1];

        if(i%3 == 0){
            if(c != ':'){
                return false;                                 
            }
        

        }else{
            if( !( (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9') ) ){
                return false;
            }
        }

    }
    return true;
}

void proximoth_toolset_generate_random_mac(byte_t mac[6]){

    memset(mac,0x00,6);

    mac[0] = random() % 256; mac[0] &= 0b11111100;
    mac[1] = random() % 256;
    mac[2] = random() % 256;

    mac[3] = proximoth_toolset_powm(mac[0],30,256);
    mac[4] = proximoth_toolset_powm(mac[1],40,256);
    mac[5] = proximoth_toolset_powm(mac[2],50,256);
    return;
}

bool proximoth_toolset_confirm_generated_mac(const byte_t mac[6]){

    if( proximoth_toolset_powm(mac[0],30,256) == mac[3] && \
        proximoth_toolset_powm(mac[1],40,256) == mac[4] && \
        proximoth_toolset_powm(mac[2],50,256) == mac[5] )
    {

        return true;
    }else{
        return false;
    }


}


void proximoth_toolset_convert_mac_to_string(const byte_t mac[6], char mac_string[PROXIMOTH_MAC_STRING_SIZE]){
        snprintf(mac_string,PROXIMOTH_MAC_STRING_SIZE,"%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        mac_string[PROXIMOTH_MAC_STRING_SIZE-1] = '\0'; // Just to make sure...
}

