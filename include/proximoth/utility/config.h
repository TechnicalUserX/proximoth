#ifndef PROXIMOTH_CONFIG_H
#define PROXIMOTH_CONFIG_H

#include <proximoth/proximoth.h>
#include <proximoth/utility/error.h>
#include <proximoth/wireless/mac.h>

#define PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN 256

extern char proximoth_cts_sniffer_dump_file[PROXIMOTH_CONFIG_DUMP_FILE_MAXLEN+1];

extern pcap_dumper_t* proximoth_config_dump_handle;

extern struct timeval proximoth_config_start_time;

extern proximoth_byte_t proximoth_config_bssid_mac[6];

extern proximoth_byte_t proximoth_config_target_mac[6];

extern char proximoth_config_bssid_mac_string[PROXIMOTH_MAC_STRING_SIZE];

extern char proximoth_config_target_mac_string[PROXIMOTH_MAC_STRING_SIZE];

extern FILE* proximoth_config_file_out;

extern pcap_t* proximoth_config_interface_handle;

extern char proximoth_config_error_buffer[256];

struct proximoth_config_flags_t{
    bool i;
    bool o;
    bool b;
    bool w;
    bool r;
    bool d;
    bool a;
    bool t;
};

extern struct proximoth_config_flags_t proximoth_config_flags;

extern time_t proximoth_config_time_gmt_offset;

extern atomic_bool proximoth_config_finished;

void proximoth_config_print_usage(void);

void proximoth_config_print_version(void);

void proximoth_config(int argc, char* argv[]);

void proximoth_config_reset(void);

#endif