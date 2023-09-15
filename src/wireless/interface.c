#include <proximoth/wireless/interface.h>

int proximoth_interface_channel;

proximoth_interface_mode_t proximoth_interface_mode;

proximoth_interface_state_t proximoth_interface_state;

char proximoth_interface_name[IFNAMSIZ] = {0};

int proximoth_interface_socket = -1;

int proximoth_interface_get_mode(const char* interface_name, proximoth_interface_mode_t* mode){

    struct iwreq wrq = {0};
    int ret = -1;

	strncpy(wrq.ifr_name, interface_name, IFNAMSIZ);
    if((ret = ioctl(proximoth_interface_socket,SIOCGIWMODE,&wrq)) >= 0){
        *mode = (proximoth_interface_mode_t)wrq.u.mode;
    }
    return ret;
}

double proximoth_interface_convert_freq_to_float(struct iw_freq* frequency){
	return ((double) frequency->m) * pow(10, frequency->e);
}

int proximoth_interface_get_state(char* interface_name, proximoth_interface_state_t* state){


    struct ifreq ifr;

    strncpy(ifr.ifr_name,interface_name,IFNAMSIZ);
    int ret;
    if( (ret = ioctl(proximoth_interface_socket,SIOCGIFFLAGS,&ifr)) >= 0){

        if((ifr.ifr_flags & IFF_UP) == IFF_UP){
            *state = PROXIMOTH_INTERFACE_STATE_UP;
        }else{
            *state = PROXIMOTH_INTERFACE_STATE_DOWN;
        }

    }
    shutdown(proximoth_interface_socket,SHUT_RDWR);
    return ret;
}

int proximoth_interface_get_channel(char* interface_name, int* channel){

    struct iwreq wrq;

	char buf[sizeof(struct iw_range) * 2] = {0};

    double frequency = 0.0;

    int ret;

    strncpy(wrq.ifr_name,interface_name,IFNAMSIZ);

    if((ret = ioctl(proximoth_interface_socket,SIOCGIWFREQ,&wrq)) >= 0  ){
        frequency = (double)(wrq.u.freq.m) * pow(10,wrq.u.freq.e);
    }
    
    wrq.u.data.pointer = buf;
    wrq.u.data.length = sizeof(buf);
    wrq.u.data.flags = 0;

    if ((ret = ioctl(proximoth_interface_socket, SIOCGIWRANGE, &wrq)) >= 0)
	{
        struct iw_range* range = (struct iw_range *) buf;
        for(int i = 0; i < range->num_frequency; i++){

            if(frequency == proximoth_interface_convert_freq_to_float(&(range->freq[i])  )){
                *channel = range->freq[i].i;
                return 0;
            }

        }

        return 1;
    }else{
        return 1;
    }
    
}