#include "unity.h"

void test_status_ok_is_success(void);
void test_status_timeout_is_not_success(void);
void test_new_heartbeat_is_fresh(void);
void test_unchanged_heartbeat_times_out(void);
void test_heartbeat_wraparound_is_fresh(void);
void test_busy_ipc_flag_blocks_publish(void);

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_status_ok_is_success);
    RUN_TEST(test_status_timeout_is_not_success);
    RUN_TEST(test_new_heartbeat_is_fresh);
    RUN_TEST(test_unchanged_heartbeat_times_out);
    RUN_TEST(test_heartbeat_wraparound_is_fresh);
    RUN_TEST(test_busy_ipc_flag_blocks_publish);
    return UNITY_END();
}
