#ifndef _PROXIMOTH_INTERFACE_MGMT_HPP
#define _PROXIMOTH_INTERFACE_MGMT_HPP

#include "proximoth.hpp"

typedef enum{
    PROXIMOTH_INTERFACE_MODE_AUTO		= IW_MODE_AUTO,
	PROXIMOTH_INTERFACE_MODE_ADHOC		= IW_MODE_ADHOC,
	PROXIMOTH_INTERFACE_MODE_MANAGED	= IW_MODE_INFRA,
	PROXIMOTH_INTERFACE_MASTER	= IW_MODE_MASTER,	
	PROXIMOTH_INTERFACE_MODE_REPEAT	= IW_MODE_REPEAT,
	PROXIMOTH_INTERFACE_MODE_SECOND	= IW_MODE_SECOND,
	PROXIMOTH_INTERFACE_MODE_MONITOR	= IW_MODE_MONITOR
}proximoth_interface_mode_t;

typedef enum{
    PROXIMOTH_INTERFACE_STATE_DOWN,
    PROXIMOTH_INTERFACE_STATE_UP
}proximoth_interface_state_t;


double proximoth_convert_freq_to_float(struct iw_freq* freq);

int proximoth_interface_get_mode(const char* iface_name,proximoth_interface_mode_t* mode);

int proximoth_interface_get_state(char* iface_name, proximoth_interface_state_t* state);

int proximoth_interface_get_channel(char* iface_name, int* channel);

extern int proximoth_interface_channel;

extern proximoth_interface_mode_t proximoth_interface_mode;

extern proximoth_interface_state_t proximoth_interface_state;

extern char proximoth_interface_name[IFNAMSIZ];

#endif