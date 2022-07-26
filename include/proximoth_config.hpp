#ifndef _PROXIMOTH_CONFIG_HPP
#define _PROXIMOTH_CONFIG_HPP

#include "proximoth.hpp"

#define PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN 127

extern char proximoth_frame_sniffer_dump_file[PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN+1];

extern pcap_dumper_t* proximoth_config_dump_handle;

extern struct timeval proximoth_config_start_time;

extern byte_t proximoth_config_bssid_mac[6];

extern byte_t proximoth_config_target_mac[6];

extern FILE* proximoth_config_file_out;

extern pcap_t* proximoth_config_interface_handle;

extern char proximoth_config_error_buffer[256];

struct proximoth_config_flags_t{
    bool i = 0;
    bool f = 0;
    bool b = 0;
    bool w = 0;
    bool r = 0;
    bool d = 0;
};

extern struct proximoth_config_flags_t proximoth_config_flags;

extern time_t proximoth_config_time_gmt_offset;

extern bool proximoth_config_finished;

void proximoth_config_print_usage(void);

void proximoth_config_print_version(void);

int proximoth_config(int argc, char* argv[]);


#endif