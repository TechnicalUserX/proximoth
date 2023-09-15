#ifndef PROXIMOTH_TOOLSET_H
#define PROXIMOTH_TOOLSET_H

#include <proximoth/proximoth.h>
#include <proximoth/misc/types.h>

#define PROXIMOTH_MAC_STRING_SIZE 18 // 17 + 1(Null Terminator)

typedef proximoth_byte_t proximoth_mac_t[6];

typedef char proximoth_mac_string_t[PROXIMOTH_MAC_STRING_SIZE];


bool proximoth_mac_validate_mac_string(const char* mac_string);

void proximoth_mac_generate_random_mac(proximoth_mac_t);

void proximoth_mac_convert_mac_to_string(const proximoth_mac_t,proximoth_mac_string_t);

#endif