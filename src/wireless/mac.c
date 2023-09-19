#include <proximoth/wireless/mac.h>

bool proximoth_mac_validate_mac_string(const char* mac_string){
    if(mac_string == NULL){
        return false;
    }

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

void proximoth_mac_generate_random_mac(proximoth_mac_t mac){

    memset(mac,0x00,6);
    mac[0] = random() % 256; 
    mac[1] = random() % 256;
    mac[2] = random() % 256;
    mac[3] = random() % 256;
    mac[4] = random() % 256;
    mac[5] = random() % 256;
    return;
}

void proximoth_mac_convert_mac_to_string(const proximoth_mac_t mac, proximoth_mac_string_t mac_string){
        snprintf(mac_string,PROXIMOTH_MAC_STRING_SIZE,"%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        mac_string[PROXIMOTH_MAC_STRING_SIZE-1] = '\0'; // Just to make sure...
}
