#ifndef PROXIMOTH_INTERFACE_H
#define PROXIMOTH_INTERFACE_H

#include <proximoth/proximoth.h>

typedef enum{
    PROXIMOTH_INTERFACE_MODE_AUTO		= IW_MODE_AUTO,
	PROXIMOTH_INTERFACE_MODE_ADHOC		= IW_MODE_ADHOC,
	PROXIMOTH_INTERFACE_MODE_MANAGED	= IW_MODE_INFRA,
	PROXIMOTH_INTERFACE_MASTER			= IW_MODE_MASTER,	
	PROXIMOTH_INTERFACE_MODE_REPEAT		= IW_MODE_REPEAT,
	PROXIMOTH_INTERFACE_MODE_SECOND		= IW_MODE_SECOND,
	PROXIMOTH_INTERFACE_MODE_MONITOR	= IW_MODE_MONITOR
}proximoth_interface_mode_t;

typedef enum{
    PROXIMOTH_INTERFACE_STATE_DOWN,
    PROXIMOTH_INTERFACE_STATE_UP
}proximoth_interface_state_t;


extern int proximoth_interface_channel;

extern proximoth_interface_mode_t proximoth_interface_mode;

extern proximoth_interface_state_t proximoth_interface_state;

extern char proximoth_interface_name[IFNAMSIZ];

extern int proximoth_interface_socket;

double proximoth_interface_convert_freq_to_float(struct iw_freq* frequency);

int proximoth_interface_get_mode(const char* interface_name,proximoth_interface_mode_t* mode);

int proximoth_interface_get_state(char* interface_name, proximoth_interface_state_t* state);

int proximoth_interface_get_channel(char* interface_name, int* channel);


#endif