#ifndef _PROXIMOTH_MISC_TOOLS_HPP
#define _PROXIMOTH_MISC_TOOLS_HPP

#include "proximoth.hpp"

uint64_t proximoth_toolset_powm(uint64_t base, uint64_t pow, uint64_t modulo);

bool proximoth_toolset_validate_mac_address(const char* mac_string);

void proximoth_toolset_generate_random_mac(byte_t mac[6]);

bool proximoth_toolset_confirm_generated_mac(byte_t mac[6]);

std::string proximoth_toolset_convert_mac_to_string(byte_t mac[6]);

#endif