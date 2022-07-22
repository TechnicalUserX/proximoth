#ifndef _PROXIMOTH_ERROR_HPP
#define _PROXIMOTH_ERROR_HPP

#include "proximoth.hpp"

#define PROXIMOTH_SUCCESS 0x0
#define PROXIMOTH_ERROR_GENERIC 0x1
#define PROXIMOTH_ERROR_INTERFACE_NOT_FOUND 0x0101
#define PROXIMOTH_ERROR_INTERFACE_NO_MONITOR 0x0102
#define PROXIMOTH_ERROR_INTERFACE_CANNOT_OPEN 0x0103
#define PROXIMOTH_ERROR_INTERFACE_NOT_UP 0x0104
#define PROXIMOTH_ERROR_INTERFACE_NOT_SPECIFIED 0x0105
#define PROXIMOTH_ERROR_BSSID_MAC_INVALID 0x0201
#define PROXIMOTH_ERROR_TARGET_MAC_INVALID 0x0202
#define PROXIMOTH_ERROR_FILE_CANNOT_CREATE 0x0402
#define PROXIMOTH_ERROR_THREADS_CANNOT_START 0x0502
#define PROXIMOTH_ERROR_THREADS_BAD_JOIN    0x0504
#define PROXIMOTH_ERROR_OPTION_INVALID 0x0601
#define PROXIMOTH_ERROR_OPTION_NEEDS_ARG 0x0602
#define PROXIMOTH_ERROR_PROCESS_CHILD_BAD_KILL 0x0701
#define PROXIMOTH_ERROR_PCAP_FILE_CANNOT_CREATE 0x0801
#define PROXIMOTH_ERROR_SIGNAL_BAD_INIT 0x0901
#define PROXIMOTH_ERROR_IOCTL           0x0A01
std::string proximoth_error_get_message(int error_code);

#endif