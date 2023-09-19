// Proximoth Test 001
// Test MAC address manipulation functions

#include <gtest/gtest.h>
#include <proximoth/wireless/mac.h>
#include <proximoth/utility/error.h>

TEST(proximoth_wireless,mac_validation){

    ASSERT_TRUE(proximoth_mac_validate_mac_string("aa:bb:cc:00:11:22"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string("jskladfjlaksdjflasdjflaskdjfasjkdlf"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string("xx:cc:f$:??:0h:32"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string("a"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string("aabbccddeeff"));

    ASSERT_TRUE(proximoth_mac_validate_mac_string("ff:FF:aa:AA:BB:cc"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string("?-#"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string("12345"));

    ASSERT_FALSE(proximoth_mac_validate_mac_string(""));

    ASSERT_FALSE(proximoth_mac_validate_mac_string(NULL));

}


TEST(proximoth_wireless,mac_to_str_convert){

    proximoth_mac_t mac_1 = {0xFF, 0x12, 0xBA, 0x31, 0x73, 0xDC};
    proximoth_mac_t mac_2 = {0xAB, 0x99, 0x54, 0x52, 0x60, 0x31};
    proximoth_mac_t mac_3 = {0x05, 0x00, 0xFC, 0x69, 0x41, 0xE2};


    proximoth_mac_string_t macstr_1 = {0};
    proximoth_mac_string_t macstr_2 = {0};
    proximoth_mac_string_t macstr_3 = {0};

    proximoth_mac_string_t macstr_1_chk = "FF:12:BA:31:73:DC";
    proximoth_mac_string_t macstr_2_chk = "AB:99:54:52:60:31";
    proximoth_mac_string_t macstr_3_chk = "05:00:FC:69:41:E2";

    proximoth_mac_convert_mac_to_string(mac_1,macstr_1);
    proximoth_mac_convert_mac_to_string(mac_2,macstr_2);
    proximoth_mac_convert_mac_to_string(mac_3,macstr_3);


    ASSERT_STREQ(macstr_1, macstr_1_chk);
    ASSERT_STREQ(macstr_2, macstr_2_chk);
    ASSERT_STREQ(macstr_3, macstr_3_chk);

}

