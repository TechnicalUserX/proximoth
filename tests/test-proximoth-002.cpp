// Proximoth Test 002
// Test error message retrieval functionality

#include <gtest/gtest.h>
#include <proximoth/utility/error.h>

TEST(proximoth_utility,error_get_message){

    char error_1_chk[] = "Cannot set filter for interface.";
    char error_2_chk[] = "Interface is not in monitor mode.";
    char error_3_chk[] = "Couldn't start the threads.";

    ASSERT_STREQ(proximoth_error_get_message(PROXIMOTH_ERROR_INTERFACE_CANNOT_SET_FILTER),error_1_chk);
    ASSERT_STREQ(proximoth_error_get_message(PROXIMOTH_ERROR_INTERFACE_NO_MONITOR),error_2_chk);
    ASSERT_STREQ(proximoth_error_get_message(PROXIMOTH_ERROR_THREADS_CANNOT_START),error_3_chk);

}