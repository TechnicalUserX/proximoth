#ifndef PROXIMOTH_TOOLSET_H
#define PROXIMOTH_TOOLSET_H

#include "proximoth.h"

uint64_t proximoth_toolset_powm(uint64_t base, uint64_t pow, uint64_t modulo);

bool proximoth_toolset_validate_mac_string(const char* mac_string);

void proximoth_toolset_generate_random_mac(byte_t mac[6]);

bool proximoth_toolset_confirm_generated_mac(const byte_t mac[6]);

void proximoth_toolset_convert_mac_to_string(const byte_t mac[6],char mac_string[PROXIMOTH_MAC_STRING_SIZE]);

#endif